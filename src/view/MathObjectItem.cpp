#include "view/MathObjectItem.h"
#include <QPainter>

MathObjectItem::MathObjectItem(const MathObject &obj, QGraphicsItem *parent)
    : QGraphicsItem(parent), m_obj(obj) {}

QRectF MathObjectItem::boundingRect() const {
    return QRectF(0, 0, WIDTH, HEIGHT);
}

void MathObjectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    painter->setPen(Qt::black);
    painter->setBrush(Qt::lightGray);
    painter->drawRect(boundingRect());
    painter->drawText(boundingRect(), Qt::AlignCenter, "[F]");
}

void MathObjectItem::setObject(const MathObject &obj) { m_obj = obj; update(); }
MathObject MathObjectItem::object() const { return m_obj; }