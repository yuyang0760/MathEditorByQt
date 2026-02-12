#include "view/DocumentView.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QFontMetrics>

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
    
    // 设置一个默认场景矩形，防止显示问题
    m_scene->setSceneRect(0, 0, 800, 600);
}

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

Document *DocumentView::document() const
{
    return m_document;
}

void DocumentView::setSelection(const Selection &selection)
{
    m_selection = selection;
    updateLayout();
}

Selection DocumentView::selection() const
{
    return m_selection;
}

Cursor *DocumentView::cursor() const
{
    return m_cursor;
}

void DocumentView::updateLayout()
{
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
    qreal y = 10;
    qreal lineHeight = 20;
    
    for (int i = 0; i < m_document->paragraphCount(); i++)
    {
        Paragraph paragraph = m_document->paragraph(i);
        QString text = paragraph.text();
        
        // 创建文本项
        QGraphicsTextItem *textItem = new QGraphicsTextItem(text);
        textItem->setPos(10, y);
        m_scene->addItem(textItem);
        
        y += lineHeight;
    }
    
    // 更新光标位置
    if (m_cursor)
    {
        QPoint point = pointFromPosition(m_cursor->position());
        m_cursor->setPos(point.x(), point.y());
    }
    
    // 确保场景矩形至少有一个最小尺寸，防止窗口显示异常
    QRectF sceneRect = m_scene->itemsBoundingRect().adjusted(-10, -10, 10, 10);
    if (sceneRect.isNull() || sceneRect.isEmpty()) {
        sceneRect = QRectF(0, 0, 800, 600); // 设置默认大小
    }
    m_scene->setSceneRect(sceneRect);
}

void DocumentView::ensureCursorVisible()
{
    if (m_cursor)
    {
        QRectF cursorRect = m_cursor->boundingRect();
        cursorRect.moveTo(m_cursor->pos());
        ensureVisible(cursorRect);
    }
}

void DocumentView::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);
    
    if (event->button() == Qt::LeftButton)
    {
        QPoint point = event->pos();
        m_selectionStart = positionFromPoint(point);
        m_selecting = true;
        
        // 设置初始选择
        Selection selection(m_selectionStart, m_selectionStart);
        setSelection(selection);
        
        // 更新光标位置
        m_cursor->setPosition(m_selectionStart);
        m_cursor->show();
        m_cursor->stopBlinking();
    }
}

void DocumentView::mouseMoveEvent(QMouseEvent *event)
{
    QGraphicsView::mouseMoveEvent(event);
    
    if (m_selecting && (event->buttons() & Qt::LeftButton))
    {
        QPoint point = event->pos();
        Selection::Position endPos = positionFromPoint(point);
        
        // 更新选择
        Selection selection(m_selectionStart, endPos);
        setSelection(selection);
    }
}

void DocumentView::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);
    
    if (event->button() == Qt::LeftButton && m_selecting)
    {
        m_selecting = false;
        m_cursor->startBlinking();
    }
}

void DocumentView::keyPressEvent(QKeyEvent *event)
{
    QGraphicsView::keyPressEvent(event);
    
    // 处理键盘事件，如方向键移动光标等
    // 这里可以添加具体的键盘事件处理逻辑
}

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

QPoint DocumentView::pointFromPosition(const Selection::Position &position) const
{
    // 简单的位置到点转换
    // 实际实现中需要更精确的计算
    qreal x = 10 + position.position * 8;
    qreal y = 10 + position.paragraph * 20;
    
    return QPoint(static_cast<int>(x), static_cast<int>(y));
}
