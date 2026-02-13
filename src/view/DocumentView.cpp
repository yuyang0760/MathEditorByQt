// ============================================================================
// DocumentView.cpp
// 文档视图类的实现文件
// 负责文档渲染、用户交互和输入法支持
// ============================================================================

#include "view/DocumentView.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFontMetrics>
#include <QGuiApplication>
#include <QInputMethod>

/**
 * @brief 构造函数
 * @param parent 父窗口部件
 */
DocumentView::DocumentView(QWidget *parent)
    : QGraphicsView(parent),
      m_scene(new QGraphicsScene(this)),          // 图形场景，用于管理文档的可视化元素
      m_document(nullptr),                        // 文档指针，初始化为空，后续通过setDocument设置
      m_cursor(new Cursor(this)),                 // 光标对象，用于显示和控制文本插入点
      m_selecting(false),                         // 选择状态标志，用于跟踪鼠标拖拽选择操作
      m_composingTextItem(nullptr) // 组合文本项指针，用于显示输入法的临时组合字符
{
    setScene(m_scene); // 设置当前视图的场景为m_scene
    m_scene->addItem(m_cursor); // 将光标对象添加到场景中
    setDragMode(NoDrag); // 禁用拖拽模式
    setRenderHint(QPainter::Antialiasing); // 启用抗锯齿渲染
    setAlignment(Qt::AlignLeft | Qt::AlignTop); // 设置文本对齐方式为左对齐和顶部对齐
    // 设置一个默认场景矩形，防止显示问题
    m_scene->setSceneRect(0, 0, 800, 600);

    // 设置蓝色边框，方便区分DocumentView的大小
    setStyleSheet("border: 2px solid blue; background-color: lightgray;");
}

/**
 * @brief 设置文档
 * @param document 文档指针
 */
void DocumentView::setDocument(Document *document)
{
    m_document = document;
    
    // 如果有文档，设置光标到文档开头
    if (m_document && m_cursor) {
        Selection::Position startPos;
        startPos.paragraph = 0;
        startPos.position = 0;
        m_cursor->setPosition(startPos);
    }
    
    updateLayout();
}

/**
 * @brief 获取文档
 * @return 文档指针
 */
Document *DocumentView::document() const
{
    return m_document;
}

/**
 * @brief 设置选择
 * @param selection 选择范围
 */
void DocumentView::setSelection(const Selection &selection)
{
    m_selection = selection;
    updateLayout();
}

/**
 * @brief 获取选择
 * @return 选择范围
 */
Selection DocumentView::selection() const
{
    return m_selection;
}

/**
 * @brief 获取光标
 * @return 光标指针
 */
Cursor *DocumentView::cursor() const
{
    return m_cursor;
}

/**
 * @brief 更新布局
 * 重新布局文档内容并更新光标位置
 * 1. 首先检查场景和文档是否有效，无效则直接返回
 * 2. 清除场景中除光标外的所有图形项
 * 3. 遍历文档中的每个段落，创建新的文本图形项并添加到场景中
 * 4. 更新光标位置到当前文档位置
 * 5. 确保场景有合理的矩形范围，防止显示问题
 * 6. 更新输入法系统，通知光标位置变化
 * TODO: 局部更新，只更新受影响的段落
 */
void DocumentView::updateLayout() {
    // 检查场景和文档是否有效
    if (!m_scene || !m_document)
        return;

    // 清除场景中的所有项目，除了光标（我们稍后会单独处理它）
    QList<QGraphicsItem *> items = m_scene->items();
    for (QGraphicsItem *item : items) {
        if (item != m_cursor) {  // 不删除光标项
            m_scene->removeItem(item);
            delete item;
        }
    }

    // 简单的文档布局
    qreal y = 10; // 起始Y坐标
    qreal lineHeight = 20; // 行高

    // 遍历文档中的所有段落
    for (int i = 0; i < m_document->paragraphCount(); i++) {
        Paragraph paragraph = m_document->paragraph(i);
        QString text = paragraph.text();

        // 创建文本项并设置位置
        QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
        textItem->setPos(1, y);
        m_scene->addItem(textItem);

        y += lineHeight; // 移动到下一行
    }

    // 更新光标位置
    if (m_cursor) {
        QPoint point = pointFromPosition(m_cursor->position());
        m_cursor->setPos(point.x(), point.y());
        // 通知输入法系统光标位置已更新
        updateInputMethod();
    }
    
    // 确保场景矩形至少有一个最小尺寸，防止窗口显示异常
    QRectF sceneRect = m_scene->itemsBoundingRect().adjusted(-10, -10, 10, 10);
    if (sceneRect.isNull() || sceneRect.isEmpty()) {
        sceneRect = QRectF(0, 0, 800, 600); // 设置默认大小
    }
    m_scene->setSceneRect(sceneRect);
}

/**
 * @brief 确保光标可见
 */
void DocumentView::ensureCursorVisible()
{
    if (m_cursor)
    {
        QRectF cursorRect = m_cursor->boundingRect();
        cursorRect.moveTo(m_cursor->pos());
        ensureVisible(cursorRect);
    }
}

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件
 */
void DocumentView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    
    if (event->button() == Qt::LeftButton)
    {
        // 确保获得焦点
        setFocus();
        
        QPoint point = event->pos();
        m_selectionStart = positionFromPoint(point);
        m_selecting = true;
        
        // 设置初始选择
        Selection selection(m_selectionStart, m_selectionStart);
        setSelection(selection);
        
        // 更新光标位置并确保可见
        m_cursor->setPosition(m_selectionStart);
        
        // 更新光标在场景中的实际位置
        QPoint cursorPoint = pointFromPosition(m_selectionStart);
        m_cursor->setPos(cursorPoint.x(), cursorPoint.y());
        
        m_cursor->show();
        m_cursor->stopBlinking();
        
        // 确保光标在视图内可见
        ensureCursorVisible();
        
        // 通知输入法系统光标位置已更新
        updateInputMethod();
    }
}

/**
 * @brief 鼠标移动事件处理
 * @param event 鼠标事件
 */
void DocumentView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    
    // 获取鼠标位置
    QPoint viewPos = event->pos();
    QPointF scenePos = mapToScene(viewPos);
    
    // 发射鼠标位置变化信号
    emit mousePositionChanged(scenePos, viewPos);
    
    if (m_selecting && (event->buttons() & Qt::LeftButton))
    {
        QPoint point = event->pos();
        Selection::Position endPos = positionFromPoint(point);
        
        // 更新选择
        Selection selection(m_selectionStart, endPos);
        setSelection(selection);
    }
}

/**
 * @brief 鼠标释放事件处理
 * @param event 鼠标事件
 */
void DocumentView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    
    if (event->button() == Qt::LeftButton && m_selecting)
    {
        m_selecting = false;
        
        // 确保光标在正确位置
        QPoint point = event->pos();
        Selection::Position cursorPos = positionFromPoint(point);
        m_cursor->setPosition(cursorPos);
        
        // 更新光标在场景中的实际位置
        QPoint cursorPoint = pointFromPosition(cursorPos);
        m_cursor->setPos(cursorPoint.x(), cursorPoint.y());
        
        // 开始闪烁
        m_cursor->startBlinking();
        
        // 确保光标在视图内可见
        ensureCursorVisible();
        
        // 确保获得焦点，以便输入法能够正常工作
        setFocus();
        updateInputMethod();
    }
}

/**
 * @brief 键盘按下事件处理
 * @param event 键盘事件
 */
void DocumentView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    
    // 处理键盘事件，如方向键移动光标等
    // 这里可以添加具体的键盘事件处理逻辑
}

/**
 * @brief 从点转换为位置
 * @param point 点坐标
 * @return 文档位置
 */
Selection::Position DocumentView::positionFromPoint(const QPoint &point) const
{
    // 简单的点到位置转换
    // 实际实现中需要更精确的计算
    Selection::Position pos;
    pos.paragraph = qMax(0, static_cast<int>((point.y() - 10) / 20));
    pos.position = qMax(0, static_cast<int>((point.x() - 10) / 8));
    
    // 确保位置在有效范围内
    if (m_document)
    {
        if (pos.paragraph >= m_document->paragraphCount())
        {
            pos.paragraph = qMax(0, m_document->paragraphCount() - 1);
            // 检查是否仍有段落数量
            if (m_document->paragraphCount() > 0) {
                Paragraph paragraph = m_document->paragraph(pos.paragraph);
                pos.position = paragraph.length();
            } else {
                pos.paragraph = 0;
                pos.position = 0;
            }
        }
        else
        {
            Paragraph paragraph = m_document->paragraph(pos.paragraph);
            pos.position = qMin(pos.position, paragraph.length());
        }
    }
    else
    {
        pos.paragraph = 0;
        pos.position = 0;
    }
    
    return pos;
}

/**
 * @brief 从位置转换为点
 * @param position 文档位置
 * @return 点坐标
 */
QPoint DocumentView::pointFromPosition(const Selection::Position &position) const
{
    // 简单的位置到点转换
    // 实际实现中需要更精确的计算
    qreal x = 10 + position.position * 8;
    qreal y = 10 + position.paragraph * 20;
    
    return QPoint(static_cast<int>(x), static_cast<int>(y));
}

/**
 * @brief 输入法查询
 * @param query 查询类型
 * @return 查询结果
 */
QVariant DocumentView::inputMethodQuery(Qt::InputMethodQuery query) const
{
    if (!m_cursor)
        return QGraphicsView::inputMethodQuery(query);
    
    switch (query)
    {
    case Qt::ImCursorRectangle:
    {
        // 获取光标位置（场景坐标）
        QPoint cursorPos = pointFromPosition(m_cursor->position());
        // 转换为视图坐标
        QPoint viewPos = mapFromScene(cursorPos);
        // 光标矩形（宽度2像素，高度20像素）
        return QRect(viewPos.x(), viewPos.y(), 2, 20);
    }
    case Qt::ImFont:
    {
        // 返回默认字体
        QFont font;
        font.setFamily("Microsoft YaHei");
        font.setPointSize(12);
        return font;
    }
    case Qt::ImCursorPosition:
    {
        // 返回光标在段落中的位置
        // 这里简化处理，返回段落内的字符位置
        return m_cursor->position().position;
    }
    case Qt::ImSurroundingText:
    {
        // 返回光标周围的文本（当前段落文本）
        if (m_document && m_cursor->position().paragraph < m_document->paragraphCount())
        {
            Paragraph paragraph = m_document->paragraph(m_cursor->position().paragraph);
            return paragraph.text();
        }
        return QString();
    }
    case Qt::ImAnchorPosition:
        // 锚点位置（通常与光标位置相同）
        return m_cursor->position().position;
    case Qt::ImMaximumTextLength:
        // 最大文本长度
        return 1000;
    case Qt::ImHints:
        // 输入法提示：允许多行文本
        return QVariant(static_cast<int>(Qt::ImhMultiLine));
    default:
        return QGraphicsView::inputMethodQuery(query);
    }
}

/**
 * @brief 更新输入法
 * 通知输入法系统更新查询
 */
void DocumentView::updateInputMethod()
{
    // 通知输入法系统更新查询
    QInputMethod *im = QGuiApplication::inputMethod();
    if (im) {
        im->update(Qt::ImQueryAll);
    }
}

/**
 * @brief 公开的输入法查询
 * @param query 查询类型
 * @return 查询结果
 */
QVariant DocumentView::inputMethodQueryPublic(Qt::InputMethodQuery query) const
{
    return inputMethodQuery(query);
}

/**
 * @brief 绘制组合文本
 * @param text 组合文本
 * @param position 位置
 */
void DocumentView::drawComposingText(const QString &text, const Selection::Position &position)
{
    // 清除之前的组合文本
    clearComposingText();
    
    if (text.isEmpty()) {
        return;
    }
    
    // 创建组合文本显示项
    m_composingTextItem = new QGraphicsTextItem(text);
    
    // 设置组合文本样式（通常为带下划线的灰色文本）
    QFont font = m_composingTextItem->font();
    font.setFamily("Microsoft YaHei");
    font.setPointSize(12);
    m_composingTextItem->setFont(font);
    
    QColor composingColor(128, 128, 128); // 灰色
    m_composingTextItem->setDefaultTextColor(composingColor);
    
    // 设置位置（在光标位置显示）
    QPoint pos = pointFromPosition(position);
    m_composingTextItem->setPos(pos.x(), pos.y());
    
    // 添加到场景
    m_scene->addItem(m_composingTextItem);
    
    // 保存当前组合文本
    m_currentComposingText = text;
}

/**
 * @brief 清除组合文本
 */
void DocumentView::clearComposingText()
{
    if (m_composingTextItem) {
        m_scene->removeItem(m_composingTextItem);
        delete m_composingTextItem;
        m_composingTextItem = nullptr;
    }
    m_currentComposingText.clear();
}

/**
 * @brief 显示组合文本
 * @param text 组合文本
 */
void DocumentView::showComposingText(const QString &text)
{
    if (m_cursor && !text.isEmpty()) {
        drawComposingText(text, m_cursor->position());
    }
}

/**
 * @brief 隐藏组合文本
 */
void DocumentView::hideComposingText()
{
    clearComposingText();
}
