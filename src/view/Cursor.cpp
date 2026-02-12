#include "view/Cursor.h"
#include <QPainter>

Cursor::Cursor(QObject *parent)
    : QObject(parent), QGraphicsItem(), m_visible(true), m_blinkTimer(new QTimer(this))
{
    connect(m_blinkTimer, &QTimer::timeout, this, &Cursor::toggleVisibility);
}

void Cursor::setPosition(const Selection::Position &position)
{
    m_position = position;
    update();
}

Selection::Position Cursor::position() const
{
    return m_position;
}

void Cursor::show()
{
    m_visible = true;
    update();
}

void Cursor::hide()
{
    m_visible = false;
    update();
}

bool Cursor::isVisible() const
{
    return m_visible;
}

void Cursor::startBlinking()
{
    m_blinkTimer->start(BLINK_INTERVAL);
}

void Cursor::stopBlinking()
{
    m_blinkTimer->stop();
    m_visible = true;
    update();
}

QRectF Cursor::boundingRect() const
{
    return QRectF(0, 0, 2, 20);
}

void Cursor::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    
    if (m_visible)
    {
        painter->setPen(QPen(Qt::black, 2));
        painter->drawLine(1, 0, 1, 20);
    }
}

void Cursor::toggleVisibility()
{
    m_visible = !m_visible;
    update();
}
