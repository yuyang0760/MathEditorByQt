// ============================================================================
// MathObjectItem.cpp
// 数学对象图形项实现文件
// 实现数学对象的图形化显示
// ============================================================================

#include "view/MathObjectItem.h"
#include <QPainter>

/**
 * @brief 构造函数
 * 
 * 初始化数学对象图形项，设置父项和数学对象
 * 
 * @param obj 数学对象
 * @param parent 父图形项
 */
MathObjectItem::MathObjectItem(const MathObject &obj, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_obj(obj) {}

/**
 * @brief 获取边界矩形
 * 
 * @return 图形项的边界矩形
 */
QRectF MathObjectItem::boundingRect() const {
    return QRectF(0, 0, WIDTH, HEIGHT);
}

/**
 * @brief 绘制图形项
 * 
 * 绘制数学对象的占位符显示，使用灰色矩形和"[F]"文本
 * 
 * @param painter 绘图设备
 * @param option 样式选项
 * @param widget 绘制部件
 */
void MathObjectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(Qt::black);
    painter->setBrush(Qt::lightGray);
    painter->drawRect(boundingRect());
    painter->drawText(boundingRect(), Qt::AlignCenter, "[F]");
}

/**
 * @brief 设置数学对象
 * 
 * @param obj 新的数学对象
 */
void MathObjectItem::setObject(const MathObject &obj) { m_obj = obj; update(); }

/**
 * @brief 获取当前数学对象
 * 
 * @return 当前数学对象
 */
MathObject MathObjectItem::object() const { return m_obj; }