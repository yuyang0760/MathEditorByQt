#ifndef GRAPHICSTEXTITEM_H
#define GRAPHICSTEXTITEM_H

#include <QGraphicsItem>
#include <QFont>
#include <QInputMethodEvent>

class GraphicsTextItem : public QGraphicsItem
{
public:
    GraphicsTextItem(QGraphicsItem *parent = nullptr);

    // QGraphicsItem 接口
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    // 事件处理
    void keyPressEvent(QKeyEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

    // 文本操作
    void setText(const QString &text);
    QString getText() const;

    // 光标操作
    void setCursorPosition(int position);
    int getCursorPosition() const;
    void setCursorVisible(bool visible);
    void setActive(bool active);
    bool isActive() const;

    // 输入法支持
    void setComposingText(const QString &text);
    void clearComposingText();

    // 编辑操作
    void insertText(const QString &text);
    void backspace();
    void deleteChar();
    void moveCursorLeft();
    void moveCursorRight();
    void moveCursorToStart();
    void moveCursorToEnd();
    void replaceText(int start, int length, const QString &text);

    // 焦点事件
    void focusInEvent(QFocusEvent *event) override;
    void focusOutEvent(QFocusEvent *event) override;

    // 光标位置计算
    int getCursorPositionAt(qreal x) const;
    QRectF getCursorRect() const;
    QFont getFont() const;

private:
    QString text;
    QString composingText;
    int cursorPosition;
    bool cursorVisible;
    bool isActiveItem;
    QFont font;

    // 计算文本宽度
    qreal textWidthUpTo(int position) const;
};

#endif // GRAPHICSTEXTITEM_H