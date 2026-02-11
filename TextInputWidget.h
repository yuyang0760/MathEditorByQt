#ifndef TEXTINPUTWIDGET_H
#define TEXTINPUTWIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTimer>
#include <QInputMethodEvent>

class GraphicsTextItem;

class TextInputWidget : public QGraphicsView
{
    Q_OBJECT

public:
    TextInputWidget(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

private slots:
    void blinkCursor();

private:
    void updateCursor();
    void updateMicroFocusPosition();  // 重命名以避免混淆
    QPointF getCursorPosition() const;

    QGraphicsScene *scene;
    GraphicsTextItem *activeTextItem;
    QTimer *cursorTimer;
    bool cursorVisible;
};

#endif // TEXTINPUTWIDGET_H