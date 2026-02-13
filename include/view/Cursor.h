#ifndef CURSOR_H
#define CURSOR_H

#include "core/Selection.h"
#include <QGraphicsItem>
#include <QTimer>
#include <QObject>

class Cursor : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    Cursor(QObject *parent = nullptr, QGraphicsItem *graphicsParent = nullptr);

    void setPosition(const Position &position);
    Position position() const;

    void show();
    void hide();
    bool isVisible() const;

    void startBlinking();
    void stopBlinking();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private slots:
    void toggleVisibility();

private:
    Position m_position;
    bool m_visible;
    QTimer *m_blinkTimer;
    static const int BLINK_INTERVAL = 500;
};

#endif // CURSOR_H