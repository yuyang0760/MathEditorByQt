// ============================================================================
// Cursor.cpp
// 光标类实现文件
// 实现文档中的光标显示和闪烁效果
// ============================================================================

#include "view/Cursor.h"
#include <QPainter>
#include <QFontMetrics>

/**
 * @brief 构造函数
 * 
 * 初始化光标对象，设置父对象、图形父项、可见性和闪烁定时器
 * 
 * @param parent 父对象指针
 * @param graphicsParent 图形父项指针
 */
Cursor::Cursor(QObject *parent, QGraphicsItem *graphicsParent)
    : QObject(parent), QGraphicsItem(graphicsParent), m_visible(true), m_blinkTimer(new QTimer(this))
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
    QFont font("Microsoft YaHei", 12);
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
    QFont font("Microsoft YaHei", 12);
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