// ============================================================================
// DocumentView.cpp
// 文档视图类的实现文件
// 负责文档渲染、用户交互和输入法支持
// ============================================================================

#include "view/DocumentView.h"
#include <QGraphicsTextItem>
#include <QTextDocument>
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
    
    // 确保视图没有边框和边距
    setFrameStyle(QFrame::NoFrame);                // 无边框
    setViewportMargins(0, 0, 0, 0);               // 无内边距
    
    // 设置一个默认场景矩形，防止显示问题
    m_scene->setSceneRect(0, 0, 800, 600);

    // 禁止自身获得焦点，键盘和输入法事件应交给父部件
    setFocusPolicy(Qt::NoFocus);
    // 禁用输入法（父部件会处理）
    setAttribute(Qt::WA_InputMethodEnabled, false);
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
    if (m_selection != selection)
    {
        m_selection = selection;
        // 同步光标位置到选择起始点（通常光标就在起始位置）
        if (m_cursor)
        {
            m_cursor->setPosition(m_selection.start());
            QPointF cursorPoint = pointFromPosition(m_selection.start());
            m_cursor->setPos(cursorPoint);
        }
        updateLayout(); // 可能不需要，但保留
        emit selectionChanged(m_selection);
        // 关键：通知输入法更新
        updateInputMethod();
    }
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

    // 保存当前场景矩形，避免布局过程中场景大小变化
    QRectF oldSceneRect = m_scene->sceneRect();

    // 清除场景中的所有项目，除了光标（我们稍后会单独处理它）
    QList<QGraphicsItem *> items = m_scene->items();
    for (QGraphicsItem *item : items) {
        if (item != m_cursor) {  // 不删除光标项
            m_scene->removeItem(item);
            delete item;
        }
    }

    // 获取字体度量
    QFont font("Microsoft YaHei", 12);
    QFontMetrics metrics(font);
    int lineHeight = metrics.height();

    // 简单的文档布局
    qreal y = 10; // 起始Y坐标

    // 遍历文档中的所有段落
    for (int i = 0; i < m_document->paragraphCount(); i++) {
        Paragraph paragraph = m_document->paragraph(i);
        QString text = paragraph.text();

        // 创建文本项并设置位置和字体
        QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
        textItem->setFont(font);
        
        // 1. 清除文档边距
        textItem->document()->setDocumentMargin(0);
        
        // 2. 清除段落间距（使用样式表方法）
        textItem->document()->setDefaultStyleSheet("p { margin:0; padding:0; }");
        
        // 3. 设置文本宽度，确保边界矩形紧凑
        textItem->setTextWidth(textItem->document()->idealWidth());
        
        // 设置文本项位置
        textItem->setPos(10, y);
        m_scene->addItem(textItem);

        y += lineHeight; // 移动到下一行
    }

    // 更新光标位置
    if (m_cursor) {
        QPointF point = pointFromPosition(m_cursor->position());
        m_cursor->setPos(point);
        // 通知输入法系统光标位置已更新
        updateInputMethod();
    }
    
    // 计算新的场景矩形
    QRectF newSceneRect = m_scene->itemsBoundingRect().adjusted(-10, -10, 10, 10);
    if (newSceneRect.isNull() || newSceneRect.isEmpty()) {
        newSceneRect = QRectF(0, 0, 800, 600); // 设置默认大小
    }
    
    // 只有当新的场景矩形与旧的有显著差异时才更新
    if (!oldSceneRect.isNull() && !oldSceneRect.isEmpty()) {
        // 计算场景大小的变化
        qreal widthDiff = qAbs(newSceneRect.width() - oldSceneRect.width());
        qreal heightDiff = qAbs(newSceneRect.height() - oldSceneRect.height());
        
        // 只有当变化超过一定阈值时才更新场景矩形
        if (widthDiff > 10 || heightDiff > 10) {
            m_scene->setSceneRect(newSceneRect);
        }
    } else {
        // 如果旧的场景矩形无效，使用新的
        m_scene->setSceneRect(newSceneRect);
    }
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
        // 确保获得焦点的是父部件 TextEditorWidget
        if (parentWidget())
            parentWidget()->setFocus();

        // 原有光标定位和选择逻辑保持不变
        QPointF scenePos = mapToScene(event->pos());
        m_selectionStart = positionFromPoint(scenePos);
        m_selecting = true;
        Selection selection(m_selectionStart, m_selectionStart);
        setSelection(selection);
        m_cursor->setPosition(m_selectionStart);
        QPointF cursorPoint = pointFromPosition(m_selectionStart);
        m_cursor->setPos(cursorPoint);
        m_cursor->show();
        m_cursor->stopBlinking();
        ensureCursorVisible();
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
        QPoint point = event->pos();// 更新光标位置
        Selection::Position cursorPos = m_selection.start();
        m_cursor->setPosition(cursorPos);
        
        // 更新光标在场景中的实际位置
        QPointF cursorPoint = pointFromPosition(cursorPos);
        m_cursor->setPos(cursorPoint);
        
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
    // 忽略事件，让父部件处理
    event->ignore();
}

/**
 * @brief 从点转换为位置
 * @param point 点坐标
 * @return 文档位置
 */
Selection::Position DocumentView::positionFromPoint(const QPointF &point) const
{
    if (!m_document) return {0, 0};

    QFont font("Microsoft YaHei", 12);
    QFontMetricsF metrics(font);
    qreal lineHeight = metrics.height();
    qreal leftMargin = 10.0;

    // 计算段落索引（Y 坐标定位）
    int paraIndex = qMax(0, static_cast<int>((point.y() - leftMargin) / lineHeight));
    if (paraIndex >= m_document->paragraphCount())
        paraIndex = qMax(0, m_document->paragraphCount() - 1);

    QString text = m_document->paragraph(paraIndex).text();

    // 计算所有间隙的 X 坐标
    QList<qreal> gapPositions;
    gapPositions.append(leftMargin);
    qreal currentX = leftMargin;
    for (int i = 0; i < text.length(); ++i) {
        currentX += metrics.horizontalAdvance(text[i]);
        gapPositions.append(currentX);
    }

    // 查找最近的间隙索引
    int bestIndex = 0;
    qreal minDist = qAbs(point.x() - gapPositions[0]);
    for (int i = 1; i < gapPositions.size(); ++i) {
        qreal dist = qAbs(point.x() - gapPositions[i]);
        if (dist < minDist) {
            minDist = dist;
            bestIndex = i;
        }
    }

    return {paraIndex, bestIndex};
}

/**
 * @brief 从位置转换为点
 * @param position 文档位置
 * @return 点坐标
 */
QPointF DocumentView::pointFromPosition(const Selection::Position &position) const
{
    if (!m_document || position.paragraph >= m_document->paragraphCount())
        return QPointF(10, 10);

    QFont font("Microsoft YaHei", 12);
    QFontMetricsF metrics(font);
    qreal lineHeight = metrics.height();
    qreal leftMargin = 10.0;

    // Y 坐标
    qreal y = leftMargin + position.paragraph * lineHeight;

    // X 坐标 = 左边界 + 前 position 个字符的宽度
    QString text = m_document->paragraph(position.paragraph).text();
    int actualPos = qMin(position.position, text.length());
    qreal x = leftMargin + metrics.horizontalAdvance(text.left(actualPos));

    return QPointF(x, y);
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
        QPointF cursorPos = pointFromPosition(m_cursor->position());
        // 转换为视图坐标
        QPoint viewPos = mapFromScene(cursorPos.toPoint());
        
        // 使用动态行高计算光标矩形
        QFont font("Microsoft YaHei", 12);
        QFontMetricsF metrics(font);
        int lineHeight = metrics.height();
        
        return QRect(viewPos.x(), viewPos.y(), 2, lineHeight);
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
    QPointF pos = pointFromPosition(position);
    m_composingTextItem->setPos(pos);
    
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
