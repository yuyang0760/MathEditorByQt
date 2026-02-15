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
#include <QResizeEvent>
#include "core/CharacterFormat.h"
#include "core/StyleManager.h"

/**
 * @brief 构造函数
 */
DocumentView::DocumentView(QWidget *parent)
    : QGraphicsView(parent),
      m_scene(new QGraphicsScene(this)),
      m_document(nullptr),
      m_cursor(new Cursor(this)),
      m_selecting(false),
      m_maxWidth(800)
{
    setScene(m_scene);
    m_scene->addItem(m_cursor);
    setDragMode(NoDrag);
    setRenderHint(QPainter::Antialiasing);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setFrameStyle(QFrame::NoFrame);
    setViewportMargins(0, 0, 0, 0);
    m_scene->setSceneRect(0, 0, 800, 600);
    setFocusPolicy(Qt::NoFocus);
    setAttribute(Qt::WA_InputMethodEnabled, true);
}

/**
 * @brief 设置文档对象
 */
void DocumentView::setDocument(Document *document) {
    m_document = document;
    clearLayouts();
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
 */
Document *DocumentView::document() const { return m_document; }

/**
 * @brief 设置选择区域
 */
void DocumentView::setSelection(const Selection &selection) {
    if (m_selection == selection) return;
    
    // 清除旧的选择高亮
    QList<QGraphicsItem*> items = m_scene->items();
    for (auto item : items) {
        if (auto textItem = dynamic_cast<TextRunItem*>(item)) {
            textItem->setSelected(false);
        }
    }
    
    m_selection = selection;
    
    // 更新光标位置
    if (m_cursor) {
        m_cursor->setPosition(selection.end());
        QPointF pt = pointFromPosition(selection.end());
        m_cursor->setPos(pt);
    }
    
    // 如果选择为空，直接返回（不设置高亮）
    if (selection.isEmpty()) {
        emit selectionChanged(m_selection);
        updateInputMethod();
        return;
    }
    
    // 设置新的选择高亮（非空选择）
    Position normStart = selection.normalizedStart();
    Position normEnd = selection.normalizedEnd();
    
    for (auto item : items) {
        if (auto textItem = dynamic_cast<TextRunItem*>(item)) {
            int p = textItem->paragraphIndex();
            int itemIdx = textItem->itemIndex();
            int segStart = textItem->offsetStart();
            int segEnd = textItem->offsetEnd();
            
            Position segStartPos = {p, itemIdx, segStart};
            Position segEndPos = {p, itemIdx, segEnd};
            
            // 检查片段是否在选择范围内
            if (normStart < segEndPos && normEnd > segStartPos) {
                int selStart = 0;
                int selEnd = segEnd - segStart;
                
                if (normStart > segStartPos) {
                    selStart = normStart.offset - segStart;
                }
                if (normEnd < segEndPos) {
                    selEnd = normEnd.offset - segStart;
                }
                
                if (selStart < selEnd) {
                    textItem->setSelected(true, selStart, selEnd);
                }
            }
        }
    }
    
    emit selectionChanged(m_selection);
    updateInputMethod();
}

/**
 * @brief 获取当前选择区域
 */
Selection DocumentView::selection() const { return m_selection; }

/**
 * @brief 获取光标对象
 */
Cursor *DocumentView::cursor() const { return m_cursor; }

/**
 * @brief 获取或创建段落布局
 */
ParagraphLayout *DocumentView::getOrCreateLayout(int paragraphIndex) {
    if (!m_document || paragraphIndex < 0 || paragraphIndex >= m_document->paragraphCount()) {
        return nullptr;
    }
    
    auto it = m_paragraphLayouts.find(paragraphIndex);
    if (it != m_paragraphLayouts.end()) {
        ParagraphLayout *layout = it.value();
        if (layout->isDirty()) {
            layout->layout();
        }
        return layout;
    }
    
    ParagraphLayout *layout = new ParagraphLayout(this);
    layout->setParagraph(&m_document->paragraph(paragraphIndex));
    layout->setParagraphIndex(paragraphIndex);
    layout->setMaxWidth(m_maxWidth);
    layout->layout();
    m_paragraphLayouts.insert(paragraphIndex, layout);
    return layout;
}

/**
 * @brief 更新所有段落布局
 */
void DocumentView::updateAllLayouts() {
    if (!m_document) return;
    
    for (int i = 0; i < m_document->paragraphCount(); ++i) {
        ParagraphLayout *layout = getOrCreateLayout(i);
        if (layout) {
            layout->markDirty();
            layout->layout();
        }
    }
}

/**
 * @brief 清除所有布局缓存
 */
void DocumentView::clearLayouts() {
    qDeleteAll(m_paragraphLayouts);
    m_paragraphLayouts.clear();
}

/**
 * @brief 重建场景
 */
void DocumentView::rebuildScene() {
    clearGraphicsItems();
    if (!m_document) return;
    
    updateAllLayouts();
    
    qreal y = 10;
    qreal leftMargin = 10;
    
    for (int p = 0; p < m_document->paragraphCount(); ++p) {
        ParagraphLayout *layout = getOrCreateLayout(p);
        if (!layout) continue;
        
        const QList<Line> &lines = layout->lines();
        
        for (int l = 0; l < lines.size(); ++l) {
            const Line &line = lines[l];
            const QList<LineSegment> &segments = line.segments();
            
            for (const LineSegment &seg : segments) {
                if (seg.itemIndex >= 0 && seg.itemIndex < m_document->paragraph(p).itemCount()) {
                    const Paragraph::Item &item = m_document->paragraph(p).itemAt(seg.itemIndex);
                    
                    if (item.type == Paragraph::TextRunItem) {
                        TextRun fullRun = item.data.value<TextRun>();
                        
                        TextRun subRun;
                        subRun.setText(fullRun.text().mid(seg.offsetStart, seg.offsetEnd - seg.offsetStart));
                        subRun.setStyleId(fullRun.styleId());
                        subRun.setDirectFormat(fullRun.directFormat());
                        
                        TextRunItem *titem = new TextRunItem(subRun, p, seg.itemIndex, seg.offsetStart, seg.offsetEnd);
                        titem->setPos(leftMargin + seg.x, y + line.baseline() - seg.ascent);
                        
                        if (m_selection.isValid() && !m_selection.isEmpty()) {
                            Position segStart = {p, seg.itemIndex, seg.offsetStart};
                            Position segEnd = {p, seg.itemIndex, seg.offsetEnd};
                            
                            Position normStart = m_selection.normalizedStart();
                            Position normEnd = m_selection.normalizedEnd();
                            
                            bool shouldSelect = false;
                            int selStart = 0;
                            int selEnd = seg.offsetEnd - seg.offsetStart;
                            
                            if (normStart < segEnd && normEnd > segStart) {
                                shouldSelect = true;
                                
                                if (normStart > segStart) {
                                    selStart = normStart.offset - seg.offsetStart;
                                }
                                if (normEnd < segEnd) {
                                    selEnd = normEnd.offset - seg.offsetStart;
                                }
                            }
                            
                            if (shouldSelect && selStart < selEnd) {
                                titem->setSelected(true, selStart, selEnd);
                            }
                        }
                        
                        m_scene->addItem(titem);
                    }
                }
            }
            
            y += line.rect().height();
        }
    }
    
    if (m_cursor) {
        QPointF pt = pointFromPosition(m_cursor->position());
        m_cursor->setPos(pt);
    }
    
    // 只在场景矩形明显变化时才更新，避免视图跳动
    QRectF newRect = m_scene->itemsBoundingRect().adjusted(-10, -10, 10, 10);
    QRectF currentRect = m_scene->sceneRect();
    if (!newRect.isNull() && (newRect != currentRect)) {
        m_scene->setSceneRect(newRect);
    }
}

/**
 * @brief 清除图形项
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
 */
void DocumentView::updateLayout() {
    // 保存当前视图中心点（场景坐标）
    QPointF center = mapToScene(viewport()->rect().center());
    
    clearLayouts();
    rebuildScene();
    
    // 恢复视图中心
    centerOn(center);
    ensureCursorVisible();
}

/**
 * @brief 确保光标可见
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
        updateInputMethod();
    }
}

/**
 * @brief 鼠标移动事件处理
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
 */
void DocumentView::mouseReleaseEvent(QMouseEvent *event) {
    QGraphicsView::mouseReleaseEvent(event);
    if (event->button() == Qt::LeftButton && m_selecting) {
        m_selecting = false;
        m_cursor->startBlinking();
    }
}

/**
 * @brief 调整大小事件
 */
void DocumentView::resizeEvent(QResizeEvent *event) {
    QGraphicsView::resizeEvent(event);
    
    qreal newWidth = viewport()->width() - 20;
    if (!qFuzzyCompare(newWidth, m_maxWidth)) {
        m_maxWidth = newWidth;
        updateLayout();
    }
}

/**
 * @brief 键盘按下事件处理
 */
void DocumentView::keyPressEvent(QKeyEvent *event) {
    event->ignore();
}

/**
 * @brief 从点坐标获取文档位置
 */
Position DocumentView::positionFromPoint(const QPointF &point) const {
    if (!m_document) return {0, 0, 0};
    
    qreal searchY = point.y() - 10;
    qreal cumulativeY = 0;
    
    for (int p = 0; p < m_document->paragraphCount(); ++p) {
        DocumentView *nonConstThis = const_cast<DocumentView*>(this);
        ParagraphLayout *layout = nonConstThis->getOrCreateLayout(p);
        if (!layout) continue;
        
        const QList<Line> &lines = layout->lines();
        qreal paraHeight = layout->height();
        
        if (searchY >= cumulativeY && searchY < cumulativeY + paraHeight) {
            qreal localY = searchY - cumulativeY;
            qreal localX = point.x() - 10;
            
            int foundLine = -1;
            for (int l = 0; l < lines.size(); ++l) {
                const Line &line = lines[l];
                if (localY >= line.rect().top() && localY < line.rect().bottom()) {
                    QPointF localPoint(localX, localY);
                    return layout->positionFromPoint(localPoint);
                }
            }
            
            if (!lines.isEmpty()) {
                return lines.last().endPosition();
            }
        }
        
        cumulativeY += paraHeight;
    }
    
    if (m_document->paragraphCount() > 0) {
        int lastPara = m_document->paragraphCount() - 1;
        DocumentView *nonConstThis = const_cast<DocumentView*>(this);
        ParagraphLayout *layout = nonConstThis->getOrCreateLayout(lastPara);
        if (layout && layout->lineCount() > 0) {
            return layout->line(layout->lineCount() - 1).endPosition();
        }
    }
    
    return {0, 0, 0};
}

/**
 * @brief 从文档位置获取点坐标
 */
QPointF DocumentView::pointFromPosition(const Position &pos) const {
    if (!m_document || pos.paragraphIndex >= m_document->paragraphCount())
        return QPointF(10, 10);
    
    qreal y = 10;
    
    for (int p = 0; p < pos.paragraphIndex; ++p) {
        DocumentView *nonConstThis = const_cast<DocumentView*>(this);
        ParagraphLayout *layout = nonConstThis->getOrCreateLayout(p);
        if (layout) {
            y += layout->height();
        }
    }
    
    DocumentView *nonConstThis = const_cast<DocumentView*>(this);
    ParagraphLayout *layout = nonConstThis->getOrCreateLayout(pos.paragraphIndex);
    if (layout) {
        QPointF localPos = layout->pointFromPosition(pos);
        return QPointF(10 + localPos.x(), y + localPos.y());
    }
    
    return QPointF(10, y);
}

/**
 * @brief 输入法查询处理
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
 */
QVariant DocumentView::inputMethodQueryPublic(Qt::InputMethodQuery query) const {
    return inputMethodQuery(query);
}

/**
 * @brief 更新输入法
 */
void DocumentView::updateInputMethod() {
    QGuiApplication::inputMethod()->update(Qt::ImQueryAll);
}
