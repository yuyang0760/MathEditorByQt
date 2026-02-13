#ifndef MATHOBJECTITEM_H
#define MATHOBJECTITEM_H

#include <QGraphicsItem>
#include "core/MathObject.h"

class MathObjectItem : public QGraphicsItem {
public:
    MathObjectItem(const MathObject &obj, QGraphicsItem *parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    void setObject(const MathObject &obj);
    MathObject object() const;

    static constexpr qreal WIDTH = 30;
    static constexpr qreal HEIGHT = 20;

private:
    MathObject m_obj;
};

#endif // MATHOBJECTITEM_H