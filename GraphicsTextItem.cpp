#include "GraphicsTextItem.h"
#include <QPainter>
#include <QFontMetricsF>
#include <QDebug>
#include <QInputMethod>
#include <QKeyEvent>
#include <QFocusEvent>

GraphicsTextItem::GraphicsTextItem(QGraphicsItem *parent)
    : QGraphicsItem(parent)
    , cursorPosition(0)
    , cursorVisible(false)
    , isActiveItem(false)
{
    // 设置字体
    font.setFamily("Microsoft YaHei");
    font.setPointSize(12);

    // 设置标志
    setFlag(QGraphicsItem::ItemIsFocusable);
    setFlag(QGraphicsItem::ItemAcceptsInputMethod);
    setAcceptHoverEvents(true);
}

QRectF GraphicsTextItem::boundingRect() const
{
    QFontMetricsF metrics(font);
    qreal width = metrics.horizontalAdvance(text + composingText) + 2; // 加2给光标留空间
    qreal height = metrics.height() + 4;

    return QRectF(0, 0, width, height);
}

void GraphicsTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->setFont(font);

    // 绘制文本背景（如果激活）
    if (isActiveItem)
    {
        painter->fillRect(boundingRect(), QColor(240, 240, 255));
    }

    // 绘制文本
    painter->setPen(Qt::black);

    // 如果有预编辑文本（输入法组合），需要特殊处理
    if (!composingText.isEmpty())
    {
        // 绘制光标前的文本
        QString beforeCursor = text.left(cursorPosition);
        painter->drawText(0, font.pointSize(), beforeCursor);

        // 绘制预编辑文本（带下划线）
        QFont underlineFont = font;
        underlineFont.setUnderline(true);
        painter->setFont(underlineFont);

        qreal x = QFontMetricsF(font).horizontalAdvance(beforeCursor);
        painter->drawText(x, font.pointSize(), composingText);

        // 恢复字体
        painter->setFont(font);

        // 绘制光标后的文本
        QString afterCursor = text.mid(cursorPosition);
        x += QFontMetricsF(font).horizontalAdvance(composingText);
        painter->drawText(x, font.pointSize(), afterCursor);
    }
    else
    {
        // 没有预编辑文本，直接绘制全部文本
        painter->drawText(0, font.pointSize(), text);
    }

    // 绘制光标
    if (cursorVisible && isActiveItem)
    {
        qreal cursorX = textWidthUpTo(cursorPosition);

        painter->setPen(QPen(Qt::black, 2));
        painter->drawLine(QPointF(cursorX, 0),
                         QPointF(cursorX, QFontMetricsF(font).height()));
    }
}

void GraphicsTextItem::setText(const QString &text)
{
    this->text = text;
    prepareGeometryChange();
    update();
}

QString GraphicsTextItem::getText() const
{
    return text;
}

void GraphicsTextItem::setCursorPosition(int position)
{
    cursorPosition = qBound(0, position, text.length());
    
    // 通知输入法系统光标位置已更新
    QInputMethod *im = QGuiApplication::inputMethod();
    if (im)
    {
        im->update(Qt::ImCursorRectangle);
    }
    
    update();
}

int GraphicsTextItem::getCursorPosition() const
{
    return cursorPosition;
}

void GraphicsTextItem::setCursorVisible(bool visible)
{
    cursorVisible = visible;
    update();
}

void GraphicsTextItem::setActive(bool active)
{
    isActiveItem = active;
    cursorVisible = active;
    update();
}

bool GraphicsTextItem::isActive() const
{
    return isActiveItem;
}

void GraphicsTextItem::setComposingText(const QString &text)
{
    composingText = text;
    prepareGeometryChange();
    update();
}

void GraphicsTextItem::clearComposingText()
{
    composingText.clear();
    prepareGeometryChange();
    update();
}

void GraphicsTextItem::insertText(const QString &text)
{
    this->text.insert(cursorPosition, text);
    cursorPosition += text.length();
    clearComposingText();
    
    // 通知输入法系统文本和光标位置已更新
    QInputMethod *im = QGuiApplication::inputMethod();
    if (im)
    {
        im->update(Qt::ImQueryAll);
    }
    
    prepareGeometryChange();
    update();
}

void GraphicsTextItem::backspace()
{
    if (cursorPosition > 0)
    {
        text.remove(cursorPosition - 1, 1);
        cursorPosition--;
        clearComposingText();
        
        // 通知输入法系统文本和光标位置已更新
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImQueryAll);
        }
        
        prepareGeometryChange();
        update();
    }
}

void GraphicsTextItem::deleteChar()
{
    if (cursorPosition < text.length())
    {
        text.remove(cursorPosition, 1);
        clearComposingText();
        
        // 通知输入法系统文本和光标位置已更新
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImQueryAll);
        }
        
        prepareGeometryChange();
        update();
    }
}

void GraphicsTextItem::moveCursorLeft()
{
    if (cursorPosition > 0)
    {
        cursorPosition--;
        clearComposingText();
        
        // 通知输入法系统光标位置已更新
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImCursorRectangle);
        }
        
        update();
    }
}

void GraphicsTextItem::moveCursorRight()
{
    if (cursorPosition < text.length())
    {
        cursorPosition++;
        clearComposingText();
        
        // 通知输入法系统光标位置已更新
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImCursorRectangle);
        }
        
        update();
    }
}

void GraphicsTextItem::moveCursorToStart()
{
    cursorPosition = 0;
    clearComposingText();
    
    // 通知输入法系统光标位置已更新
    QInputMethod *im = QGuiApplication::inputMethod();
    if (im)
    {
        im->update(Qt::ImCursorRectangle);
    }
    
    update();
}

void GraphicsTextItem::moveCursorToEnd()
{
    cursorPosition = text.length();
    clearComposingText();
    
    // 通知输入法系统光标位置已更新
    QInputMethod *im = QGuiApplication::inputMethod();
    if (im)
    {
        im->update(Qt::ImCursorRectangle);
    }
    
    update();
}

void GraphicsTextItem::replaceText(int start, int length, const QString &text)
{
    if (start >= 0 && start < this->text.length())
    {
        length = qMin(length, this->text.length() - start);
        this->text.replace(start, length, text);

        if (cursorPosition >= start + length)
        {
            cursorPosition += text.length() - length;
        }
        else if (cursorPosition > start)
        {
            cursorPosition = start + text.length();
        }

        clearComposingText();
        
        // 通知输入法系统文本和光标位置已更新
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImQueryAll);
        }
        
        prepareGeometryChange();
        update();
    }
}

int GraphicsTextItem::getCursorPositionAt(qreal x) const
{
    if (text.isEmpty()) return 0;

    QFontMetricsF metrics(font);

    // 简单实现：找到最接近的字符位置
    for (int i = 0; i <= text.length(); i++)
    {
        qreal width = metrics.horizontalAdvance(text.left(i));
        if (width >= x)
        {
            return i;
        }
    }

    return text.length();
}

QRectF GraphicsTextItem::getCursorRect() const
{
    QFontMetricsF metrics(font);
    qreal cursorX = textWidthUpTo(cursorPosition);

    return QRectF(cursorX, 0, 2, metrics.height());
}

QFont GraphicsTextItem::getFont() const
{
    return font;
}

qreal GraphicsTextItem::textWidthUpTo(int position) const
{
    QFontMetricsF metrics(font);

    if (position <= cursorPosition)
    {
        // 光标前的文本
        return metrics.horizontalAdvance(text.left(position));
    }
    else
    {
        // 包括预编辑文本
        qreal baseWidth = metrics.horizontalAdvance(text.left(cursorPosition));
        baseWidth += metrics.horizontalAdvance(composingText);
        baseWidth += metrics.horizontalAdvance(text.mid(cursorPosition, position - cursorPosition));
        return baseWidth;
    }
}

void GraphicsTextItem::keyPressEvent(QKeyEvent *event)
{
    bool handled = false;

    switch (event->key())
    {
    case Qt::Key_Backspace:
        backspace();
        handled = true;
        break;

    case Qt::Key_Delete:
        deleteChar();
        handled = true;
        break;

    case Qt::Key_Left:
        moveCursorLeft();
        handled = true;
        break;

    case Qt::Key_Right:
        moveCursorRight();
        handled = true;
        break;

    case Qt::Key_Home:
        moveCursorToStart();
        handled = true;
        break;

    case Qt::Key_End:
        moveCursorToEnd();
        handled = true;
        break;
    }

    // 如果没有处理特殊键，检查是否是可打印字符
    if (!handled && !event->text().isEmpty())
    {
        // 如果有预编辑文本（输入法组合中），让输入法处理
        if (!composingText.isEmpty())
        {
            QGraphicsItem::keyPressEvent(event);
            return;
        }
        
        QString text = event->text();
        // 只接受可打印字符（过滤掉控制字符）
        for (const QChar &c : text)
        {
            if (!c.isPrint() || c.category() == QChar::Other_Control)
            {
                QGraphicsItem::keyPressEvent(event);
                return;
            }
        }
        // 插入字符
        insertText(text);
        handled = true;
    }

    if (handled)
    {
        event->accept();
    }
    else
    {
        QGraphicsItem::keyPressEvent(event);
    }
}

void GraphicsTextItem::inputMethodEvent(QInputMethodEvent *event)
{
    QString preeditString = event->preeditString();
    QString commitString = event->commitString();

    qDebug() << "GraphicsTextItem Input Method Event:";
    qDebug() << "  Preedit:" << preeditString;
    qDebug() << "  Commit:" << commitString;

    if (!preeditString.isEmpty())
    {
        // 有预编辑文本，正在输入法组合中
        composingText = preeditString;
        prepareGeometryChange();
        update();
        
        // 通知输入法系统预编辑文本已更新（光标位置可能改变）
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImCursorRectangle);
        }
    }
    else if (!commitString.isEmpty())
    {
        // 有提交文本，输入法组合完成
        // 使用insertText方法保持一致性
        insertText(commitString);
    }
    else
    {
        // 其他情况，清除预编辑文本
        composingText.clear();
        prepareGeometryChange();
        update();
        
        // 通知输入法系统预编辑文本已清除
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImCursorRectangle);
        }
    }

    event->accept();
}

QVariant GraphicsTextItem::inputMethodQuery(Qt::InputMethodQuery query) const
{
    switch (query)
    {
    case Qt::ImCursorRectangle:
    {
        // 返回光标的矩形区域（场景坐标）
        QFontMetricsF metrics(font);
        qreal cursorX = textWidthUpTo(cursorPosition);
        QRectF localRect(cursorX, 0, 2, metrics.height());
        return mapRectToScene(localRect);
    }

    case Qt::ImAnchorRectangle:
        return inputMethodQuery(Qt::ImCursorRectangle);

    case Qt::ImFont:
        return font;

    case Qt::ImCursorPosition:
        return cursorPosition;

    case Qt::ImSurroundingText:
        return text;

    case Qt::ImCurrentSelection:
        return QString();

    case Qt::ImMaximumTextLength:
        return 1000;

    case Qt::ImAnchorPosition:
        return cursorPosition;

    case Qt::ImHints:
        return QVariant(static_cast<int>(Qt::ImhNone));

    case Qt::ImPreferredLanguage:
        return "zh-CN";

    default:
        return QVariant();
    }
}

void GraphicsTextItem::focusInEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "GraphicsTextItem focusInEvent";
    // 通知输入法系统这个项目获得焦点
    QInputMethod *im = QGuiApplication::inputMethod();
    if (im)
    {
        im->update(Qt::ImQueryAll);
    }
    update();
}

void GraphicsTextItem::focusOutEvent(QFocusEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "GraphicsTextItem focusOutEvent";
    // 清除预编辑文本
    composingText.clear();
    update();
}