// ============================================================================
// DocumentView.cpp
// 文档视图实现文件
// 提供文档的图形化显示和交互功能
// ============================================================================

#include "view/DocumentView.h"
#include "view/TextRunItem.h"
#include "view/MathObjectItem.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QInputMethod>
#include <QFontMetrics>

/**
 * @brief 构造函数
 * 
 * 初始化文档视图，设置场景、光标和各种属性
 * 
 * @param parent 父部件指针
 */
DocumentView::DocumentView(QWidget *parent)
    : QGraphicsView(parent),
      m_scene(new QGraphicsScene(this)),
      m_document(nullptr),
      m_cursor(new Cursor(this)),
      m_selecting(false)
{
    setScene(m_scene);
    m_scene->addItem(m_cursor);
    setDragMode(NoDrag);
    setRenderHint(QPainter::Antialiasing);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setFrameStyle(QFrame::NoFrame);
    setViewportMargins(0, 0, 0, 0);
    m_scene->setSceneRect(0, 0, 800, 600);
    setFocusPolicy(Qt::NoFocus);          // 焦点交给父控件
    setAttribute(Qt::WA_InputMethodEnabled, true);  // 启用输入法
}

/**
 * @brief 设置文档对象
 * 
 * @param document 要设置的文档指针
 */
void DocumentView::setDocument(Document *document) {
    m_document = document;
    if (m_cursor) {
        m_cursor->setDocument(document);
    }
    if (m_document && m_cursor) {
        Position start = {0,0,0};
        m_cursor->setPosition(start);
    }
    rebuildScene();
}

/**
 * @brief 获取当前文档对象
 * 
 * @return 当前文档指针
 */
Document *DocumentView::document() const { return m_document; }

/**
 * @brief 设置选择区域
 * 
 * @param selection 新的选择对象
 */
void DocumentView::setSelection(const Selection &selection) {
    if (m_selection != selection) {
        m_selection = selection;
        if (m_cursor) {
            // 光标应该在最后松开鼠标的位置，即selection.end()
            m_cursor->setPosition(selection.end());
            QPointF pt = pointFromPosition(selection.end());
            m_cursor->setPos(pt);
        }
        rebuildScene(); // 重建场景以更新选中文本的背景颜色
        emit selectionChanged(m_selection);
        updateInputMethod();
    }
}

/**
 * @brief 获取当前选择区域
 * 
 * @return 当前选择对象
 */
Selection DocumentView::selection() const { return m_selection; }

/**
 * @brief 获取光标对象
 * 
 * @return 光标指针
 */
Cursor *DocumentView::cursor() const { return m_cursor; }

/**
 * @brief 重建场景
 * 
 * 清除现有图形项，重新创建文档内容的图形表示
 */
void DocumentView::rebuildScene() {
    clearGraphicsItems();
    if (!m_document) return;

    QFont font("Microsoft YaHei", 12);
    QFontMetrics metrics(font);
    int lineHeight = metrics.height();
    qreal y = 10;

    for (int p = 0; p < m_document->paragraphCount(); ++p) {
        Paragraph &para = m_document->paragraph(p);
        qreal x = 10;
        for (int i = 0; i < para.itemCount(); ++i) {
            auto &item = para.itemAt(i);
            QGraphicsItem *gitem = nullptr;
            if (item.type == Paragraph::TextRunItem) {
                TextRun run = item.data.value<TextRun>();
                TextRunItem *titem = new TextRunItem(run);
                titem->setPos(x, y);
                
                // 检查是否需要设置选中状态
                if (m_selection.isValid()) {
                    Position itemPos = {p, i, 0};
                    Position itemEndPos = {p, i, run.length()};
                    
                    // 检查当前文本项是否与选择区域重叠
                    // 四种重叠情况：
                    // 1. 选择区域包含文本项
                    // 2. 文本项包含选择区域
                    // 3. 选择区域与文本项部分重叠
                    // 4. 文本项与选择区域部分重叠
                    Position normStart = m_selection.normalizedStart();
                    Position normEnd = m_selection.normalizedEnd();
                    
                    bool shouldSelect = false;
                    int start = 0;
                    int end = run.length();
                    
                    // 检查重叠情况
                    if (normStart.paragraphIndex == p && normStart.itemIndex == i && normEnd.paragraphIndex == p && normEnd.itemIndex == i) {
                        // 选择区域完全在当前文本项内
                        shouldSelect = true;
                        start = normStart.offset;
                        end = normEnd.offset;
                    } else if (m_selection.contains(itemPos) || m_selection.contains(itemEndPos) ||
                              (normStart <= itemPos && normEnd >= itemEndPos)) {
                        // 选择区域与文本项有重叠
                        shouldSelect = true;
                        
                        // 调整起始位置
                        if (normStart.paragraphIndex == p && normStart.itemIndex == i) {
                            start = normStart.offset;
                        }
                        
                        // 调整结束位置
                        if (normEnd.paragraphIndex == p && normEnd.itemIndex == i) {
                            end = normEnd.offset;
                        }
                    }
                    
                    // 确保start < end
                    if (shouldSelect && start < end) {
                        titem->setSelected(true, start, end);
                    }
                }
                
                gitem = titem;
                x += titem->boundingRect().width();
            } else if (item.type == Paragraph::MathObjectItem) {
                MathObject obj = item.data.value<MathObject>();
                MathObjectItem *mitem = new MathObjectItem(obj);
                mitem->setPos(x, y);
                gitem = mitem;
                x += mitem->boundingRect().width();
            }
            if (gitem) {
                m_scene->addItem(gitem);
            }
        }
        y += lineHeight;
    }

    if (m_cursor) {
        QPointF pt = pointFromPosition(m_cursor->position());
        m_cursor->setPos(pt);
    }

    m_scene->setSceneRect(m_scene->itemsBoundingRect().adjusted(-10, -10, 10, 10));
}

/**
 * @brief 清除图形项
 * 
 * 移除并删除场景中除光标外的所有图形项
 */
void DocumentView::clearGraphicsItems() {
    QList<QGraphicsItem*> items = m_scene->items();
    for (auto item : items) {
        if (item != m_cursor) {
            m_scene->removeItem(item);
            delete item;
        }
    }
}

/**
 * @brief 更新布局
 * 
 * 重建场景并确保光标可见
 */
void DocumentView::updateLayout() {
    rebuildScene();
    ensureCursorVisible();
}

/**
 * @brief 确保光标可见
 * 
 * 调整视图，使光标在可见区域内
 */
void DocumentView::ensureCursorVisible() {
    if (m_cursor) {
        QRectF rect = m_cursor->boundingRect();
        rect.moveTo(m_cursor->pos());
        ensureVisible(rect);
    }
}

/**
 * @brief 鼠标按下事件处理
 * 
 * 处理鼠标点击事件，开始选择操作
 * 
 * @param event 鼠标事件对象
 */
void DocumentView::mousePressEvent(QMouseEvent *event) {
    QGraphicsView::mousePressEvent(event);
    if (event->button() == Qt::LeftButton) {
        if (parentWidget()) parentWidget()->setFocus();

        QPointF scenePos = mapToScene(event->pos());
        m_selectionStart = positionFromPoint(scenePos);
        m_selecting = true;
        setSelection(Selection(m_selectionStart, m_selectionStart));
        m_cursor->show();
        m_cursor->stopBlinking();
        ensureCursorVisible();
        updateInputMethod();
    }
}

/**
 * @brief 鼠标移动事件处理
 * 
 * 处理鼠标移动事件，更新选择区域
 * 
 * @param event 鼠标事件对象
 */
void DocumentView::mouseMoveEvent(QMouseEvent *event) {
    QGraphicsView::mouseMoveEvent(event);
    emit mousePositionChanged(mapToScene(event->pos()), event->pos());
    if (m_selecting && (event->buttons() & Qt::LeftButton)) {
        QPointF scenePos = mapToScene(event->pos());
        Position currentPos = positionFromPoint(scenePos);
        setSelection(Selection(m_selectionStart, currentPos));
    }
}

/**
 * @brief 鼠标释放事件处理
 * 
 * 处理鼠标释放事件，结束选择操作
 * 
 * @param event 鼠标事件对象
 */
void DocumentView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton && m_selecting) {
        m_selecting = false;
        m_cursor->startBlinking();
    }
}

/**
 * @brief 键盘按下事件处理
 * 
 * 忽略键盘事件，由父控件处理
 * 
 * @param event 键盘事件对象
 */
void DocumentView::keyPressEvent(QKeyEvent *event) {
    event->ignore();
}

/**
 * @brief 输入法查询处理
 * 
 * 处理输入法查询，返回各种输入法相关信息
 * 
 * @param query 查询类型
 * @return 查询结果
 */
QVariant DocumentView::inputMethodQuery(Qt::InputMethodQuery query) const {
    if (!m_cursor) return QGraphicsView::inputMethodQuery(query);
    
    switch (query) {
    case Qt::ImCursorRectangle: {
        QPointF cursorPos = pointFromPosition(m_cursor->position());
        QPoint viewPos = mapFromScene(cursorPos.toPoint());
        
        QFont font("Microsoft YaHei", 12);
        QFontMetricsF metrics(font);
        int lineHeight = metrics.height();
        
        return QRect(viewPos.x(), viewPos.y(), 2, lineHeight);
    }
    case Qt::ImFont: {
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(12);
        return font;
    }
    case Qt::ImCursorPosition: {
        return m_cursor->position().offset;
    }
    case Qt::ImSurroundingText: {
        if (m_document && m_cursor->position().paragraphIndex < m_document->paragraphCount()) {
            Paragraph paragraph = m_document->paragraph(m_cursor->position().paragraphIndex);
            return paragraph.plainText();
        }
        return QString();
    }
    case Qt::ImAnchorPosition:
        return m_cursor->position().offset;
    case Qt::ImMaximumTextLength:
        return 1000;
    default:
        return QGraphicsView::inputMethodQuery(query);
    }
}

/**
 * @brief 公共输入法查询方法
 * 
 * 提供外部访问输入法查询的接口
 * 
 * @param query 查询类型
 * @return 查询结果
 */
QVariant DocumentView::inputMethodQueryPublic(Qt::InputMethodQuery query) const {
    return inputMethodQuery(query);
}

/**
 * @brief 从点坐标获取文档位置
 * 
 * 将鼠标点击的屏幕坐标转换为文档中的位置
 * 
 * @param point 点坐标
 * @return 文档位置
 */
Position DocumentView::positionFromPoint(const QPointF &point) const {
    if (!m_document) return {0, 0, 0};

    QFont defaultFont("Microsoft YaHei", 12);
    QFontMetricsF metrics(defaultFont);
    qreal lineHeight = metrics.height();
    qreal leftMargin = 10.0;

    // 1. 确定段落索引（Y坐标）
    int paraIndex = qBound(0, static_cast<int>((point.y() - leftMargin) / lineHeight), m_document->paragraphCount() - 1);
    Paragraph para = m_document->paragraph(paraIndex);

    // 2. 遍历段落内的所有项，累加宽度
    qreal x = leftMargin;
    for (int i = 0; i < para.itemCount(); ++i) {
        const auto &item = para.itemAt(i);
        qreal itemWidth = 0;

        if (item.type == Paragraph::TextRunItem) {
            TextRun run = item.data.value<TextRun>();
            QFontMetricsF fm(run.format().font());
            itemWidth = fm.horizontalAdvance(run.text());
        } else if (item.type == Paragraph::MathObjectItem) {
            itemWidth = MathObjectItem::WIDTH; // 固定宽度
        }

        // 3. 检查点击点是否在当前项内
        if (point.x() < x + itemWidth) {
            // 点击在这一项内
            if (item.type == Paragraph::TextRunItem) {
                TextRun run = item.data.value<TextRun>();
                QFontMetricsF fm(run.format().font());
                QString text = run.text();
                qreal charX = x;
                // 逐字符累积宽度，找到最近的字符边界
                for (int offset = 0; offset <= text.length(); ++offset) {
                    qreal nextX = (offset < text.length()) ? charX + fm.horizontalAdvance(text[offset]) : x + itemWidth;
                    if (point.x() <= nextX) {
                        // 如果点击点距离当前字符左边更近，则放在当前字符之前；否则之后
                        qreal leftDist = point.x() - charX;
                        qreal rightDist = nextX - point.x();
                        Position pos;
                        pos.paragraphIndex = paraIndex;
                        pos.itemIndex = i;
                        pos.offset = offset + (leftDist > rightDist ? 1 : 0); // 选择最近的边界
                        return pos;
                    }
                    charX = nextX;
                }
            } else { // MathObjectItem
                // 点击公式项：根据点击位置在左半部分或右半部分决定偏移0（之前）或1（之后）
                qreal half = itemWidth / 2;
                Position pos;
                pos.paragraphIndex = paraIndex;
                pos.itemIndex = i;
                pos.offset = (point.x() < x + half) ? 0 : 1;
                return pos;
            }
        }

        x += itemWidth;
    }

    // 4. 点击在段落末尾之后：返回最后一个项的末尾
    Position pos;
    pos.paragraphIndex = paraIndex;
    pos.itemIndex = para.itemCount() - 1;
    if (pos.itemIndex >= 0) {
        const auto &lastItem = para.itemAt(pos.itemIndex);
        if (lastItem.type == Paragraph::TextRunItem) {
            TextRun run = lastItem.data.value<TextRun>();
            pos.offset = run.length();
        } else {
            pos.offset = 1; // 公式项偏移为1表示之后
        }
    } else {
        // 段落为空
        pos.itemIndex = 0;
        pos.offset = 0;
    }
    return pos;
}

/**
 * @brief 从文档位置获取点坐标
 * 
 * 将文档中的位置转换为屏幕坐标
 * 
 * @param pos 文档位置
 * @return 点坐标
 */
QPointF DocumentView::pointFromPosition(const Position &pos) const {
    if (!m_document || pos.paragraphIndex >= m_document->paragraphCount())
        return QPointF(10, 10);

    QFont defaultFont("Microsoft YaHei", 12);
    QFontMetricsF metrics(defaultFont);
    qreal lineHeight = metrics.height();
    qreal leftMargin = 10.0;

    // Y坐标
    qreal y = leftMargin + pos.paragraphIndex * lineHeight;

    // X坐标：累加直到目标项，再加上项内偏移
    Paragraph para = m_document->paragraph(pos.paragraphIndex);
    qreal x = leftMargin;
    for (int i = 0; i < pos.itemIndex; ++i) {
        const auto &item = para.itemAt(i);
        if (item.type == Paragraph::TextRunItem) {
            TextRun run = item.data.value<TextRun>();
            QFontMetricsF fm(run.format().font());
            x += fm.horizontalAdvance(run.text());
        } else { // MathObjectItem
            x += MathObjectItem::WIDTH;
        }
    }

    // 计算当前项内偏移对应的宽度
    if (pos.itemIndex >= 0 && pos.itemIndex < para.itemCount()) {
        const auto &item = para.itemAt(pos.itemIndex);
        if (item.type == Paragraph::TextRunItem) {
            TextRun run = item.data.value<TextRun>();
            QFontMetricsF fm(run.format().font());
            // 累加前 offset 个字符的宽度
            QString prefix = run.text().left(pos.offset);
            x += fm.horizontalAdvance(prefix);
        } else { // MathObjectItem
            // 偏移0表示左边界，1表示右边界
            if (pos.offset == 1)
                x += MathObjectItem::WIDTH;
        }
    }

    return QPointF(x, y);
}

/**
 * @brief 更新输入法
 * 
 * 通知输入法更新查询信息
 */
void DocumentView::updateInputMethod() {
    QGuiApplication::inputMethod()->update(Qt::ImQueryAll);
}