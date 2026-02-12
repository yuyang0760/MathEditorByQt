#ifndef CURSOR_H
#define CURSOR_H

#include "core/Selection.h"
#include <QGraphicsItem>
#include <QTimer>
#include <QObject>

class Cursor : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    Cursor(QObject *parent = nullptr);
    
    // 设置光标位置
    void setPosition(const Selection::Position &position);
    Selection::Position position() const;
    
    // 显示和隐藏光标
    void show();
    void hide();
    bool isVisible() const;
    
    // 闪烁功能
    void startBlinking();
    void stopBlinking();
    
    // 重写的方法
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    
private slots:
    void toggleVisibility();
    
private:
    Selection::Position m_position;
    bool m_visible;
    QTimer *m_blinkTimer;
    static const int BLINK_INTERVAL = 500; // 闪烁间隔（毫秒）
};

#endif // CURSOR_H
