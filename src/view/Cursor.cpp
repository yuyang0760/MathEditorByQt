#include "view/Cursor.h"
#include <QPainter>
#include <QFontMetrics>

Cursor::Cursor(QObject *parent, QGraphicsItem *graphicsParent)
    : QObject(parent), QGraphicsItem(graphicsParent), m_visible(true), m_blinkTimer(new QTimer(this))
{
    connect(m_blinkTimer, &QTimer::timeout, this, &Cursor::toggleVisibility);
}

void Cursor::setPosition(const Position &position) { m_position = position; update(); }
Position Cursor::position() const { return m_position; }
void Cursor::show() { m_visible = true; update(); }
void Cursor::hide() { m_visible = false; update(); }
bool Cursor::isVisible() const { return m_visible; }
void Cursor::startBlinking() { m_blinkTimer->start(BLINK_INTERVAL); }
void Cursor::stopBlinking() { m_blinkTimer->stop(); m_visible = true; update(); }

QRectF Cursor::boundingRect() const {
    QFont font("Microsoft YaHei", 12);
    QFontMetrics metrics(font);
    return QRectF(0, 0, 1, metrics.height());
}

void Cursor::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *) {
    if (!m_visible) return;
    QFont font("Microsoft YaHei", 12);
    QFontMetricsF metrics(font);
    painter->setPen(QPen(Qt::black, 1.0));
    painter->drawLine(QPointF(0, 0), QPointF(0, metrics.height()));
}

void Cursor::toggleVisibility() { m_visible = !m_visible; update(); }