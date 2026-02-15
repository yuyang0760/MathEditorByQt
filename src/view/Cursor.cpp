// ============================================================================
// Cursor.cpp
// 光标类实现文件
// 实现文档中的光标显示和闪烁效果
// ============================================================================

#include "view/Cursor.h"
#include <QPainter>
#include <QFontMetrics>
#include "core/Paragraph.h"
#include "core/TextRun.h"

/**
 * @brief 构造函数
 * 
 * 初始化光标对象，设置父对象、图形父项、可见性和闪烁定时器
 * 
 * @param parent 父对象指针
 * @param graphicsParent 图形父项指针
 */
Cursor::Cursor(QObject *parent, QGraphicsItem *graphicsParent)
    : QObject(parent), QGraphicsItem(graphicsParent), m_visible(true), m_blinkTimer(new QTimer(this)), m_document(nullptr)
{
    connect(m_blinkTimer, &QTimer::timeout, this, &Cursor::toggleVisibility);
}

/**
 * @brief 设置光标位置
 * 
 * @param position 新的位置
 */
void Cursor::setPosition(const Position &position) { m_position = position; update(); }

/**
 * @brief 获取当前光标位置
 * 
 * @return 当前光标位置
 */
Position Cursor::position() const { return m_position; }

/**
 * @brief 设置文档对象
 * 
 * @param document 文档指针
 */
void Cursor::setDocument(Document *document) { m_document = document; }

/**
 * @brief 获取字符格式
 * 
 * @return 光标位置字符的格式
 */
Format Cursor::getCharacterFormat() const {
    if (!m_document) {
        return Format();
    }
    
    Position pos = m_position;
    if (pos.paragraphIndex < 0 || pos.paragraphIndex >= m_document->paragraphCount()) {
        return Format();
    }
    
    Paragraph &para = m_document->paragraph(pos.paragraphIndex);
    if (pos.itemIndex < 0 || pos.itemIndex >= para.itemCount()) {
        return Format();
    }
    
    auto &item = para.itemAt(pos.itemIndex);
    if (item.type == Paragraph::TextRunItem) {
        TextRun run = item.data.value<TextRun>();
        StyleManager *styleMgr = StyleManager::instance();
        return run.effectiveFormat(styleMgr);
    }
    
    return Format();
}

/**
 * @brief 显示光标
 * 
 */
void Cursor::show() { m_visible = true; update(); }

/**
 * @brief 隐藏光标
 * 
 */
void Cursor::hide() { m_visible = false; update(); }

/**
 * @brief 检查光标是否可见
 * 
 * @return 如果光标可见返回true
 */
bool Cursor::isVisible() const { return m_visible; }

/**
 * @brief 开始光标闪烁
 * 
 */
void Cursor::startBlinking() { m_blinkTimer->start(BLINK_INTERVAL); }

/**
 * @brief 停止光标闪烁
 * 
 */
void Cursor::stopBlinking() { m_blinkTimer->stop(); m_visible = true; update(); }

/**
 * @brief 获取边界矩形
 * 
 * @return 光标的边界矩形
 */
QRectF Cursor::boundingRect() const {
    Format charFormat = getCharacterFormat();
    QFont font = (!charFormat.font().family().isEmpty()) ? charFormat.font() : QFont("Microsoft YaHei", 12);
    QFontMetrics metrics(font);
    return QRectF(0, 0, 1, metrics.height());
}

/**
 * @brief 绘制光标
 * 
 * @param painter 绘图设备
 * @param option 样式选项
 * @param widget 绘制部件
 */
void Cursor::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!m_visible) return;
    Format charFormat = getCharacterFormat();
    QFont font = (!charFormat.font().family().isEmpty()) ? charFormat.font() : QFont("Microsoft YaHei", 12);
    QFontMetricsF metrics(font);
    painter->setPen(QPen(Qt::black, 1.0));
    painter->drawLine(QPointF(0, 0), QPointF(0, metrics.height()));
}

/**
 * @brief 切换可见性
 * 
 * 用于实现光标闪烁效果
 */
void Cursor::toggleVisibility() { m_visible = !m_visible; update(); }