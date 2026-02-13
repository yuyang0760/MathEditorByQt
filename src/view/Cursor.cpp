// ============================================================================
// Cursor.cpp
// 光标类的实现文件
// 负责光标绘制、闪烁和位置管理
// ============================================================================

#include "view/Cursor.h"
#include <QPainter>

/**
 * @brief 构造函数
 * @param parent 父对象
 * @param graphicsParent 图形父项
 */
Cursor::Cursor(QObject *parent, QGraphicsItem *graphicsParent)
    : QObject(parent), QGraphicsItem(graphicsParent), m_visible(true), m_blinkTimer(new QTimer(this))
{
    connect(m_blinkTimer, &QTimer::timeout, this, &Cursor::toggleVisibility);
}

/**
 * @brief 设置光标位置
 * @param position 光标位置
 */
void Cursor::setPosition(const Selection::Position &position)
{
    m_position = position;
    update();
}

/**
 * @brief 获取光标位置
 * @return 光标位置
 */
Selection::Position Cursor::position() const
{
    return m_position;
}

/**
 * @brief 显示光标
 */
void Cursor::show()
{
    m_visible = true;
    update();
}

/**
 * @brief 隐藏光标
 */
void Cursor::hide()
{
    m_visible = false;
    update();
}

/**
 * @brief 检查光标是否可见
 * @return 是否可见
 */
bool Cursor::isVisible() const
{
    return m_visible;
}

/**
 * @brief 开始闪烁
 */
void Cursor::startBlinking()
{
    m_blinkTimer->start(BLINK_INTERVAL);
}

/**
 * @brief 停止闪烁
 */
void Cursor::stopBlinking()
{
    m_blinkTimer->stop();
    m_visible = true;
    update();
}

/**
 * @brief 获取边界矩形
 * @return 边界矩形
 */
QRectF Cursor::boundingRect() const
{
    // 获取字体度量
    QFont font("Microsoft YaHei", 12);
    QFontMetrics metrics(font);
    int height = metrics.height();
    return QRectF(0, 0, 1, height); // 调整光标宽度为1像素
}

/**
 * @brief 绘制光标
 * @param painter 绘图工具
 * @param option 样式选项
 * @param widget 部件
 */
void Cursor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    if (m_visible)
    {
        // 获取字体度量 - 使用浮点版本
        QFont font("Microsoft YaHei", 12);
        QFontMetricsF metrics(font);
        qreal height = metrics.height();
        
        painter->setPen(QPen(Qt::black, 1.0)); // 使用浮点宽度
        painter->drawLine(QPointF(0, 0), QPointF(0, height)); // 使用浮点坐标绘制
    }
}

/**
 * @brief 切换可见性
 */
void Cursor::toggleVisibility()
{
    m_visible = !m_visible;
    update();
}
