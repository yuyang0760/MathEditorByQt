#include "TextInputWidget.h"
#include "GraphicsTextItem.h"
#include <QMouseEvent>
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QDebug>
#include <QScrollBar>
#include <QApplication>

TextInputWidget::TextInputWidget(QWidget *parent)
    : QGraphicsView(parent)
    , scene(nullptr)
    , activeTextItem(nullptr)
    , cursorVisible(true)
{
    // 创建场景
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, 800, 600);
    setScene(scene);
    
    // 设置背景
    scene->setBackgroundBrush(Qt::white);
    
    // 启用键盘输入
    setFocusPolicy(Qt::StrongFocus);
    
    // 设置输入法
    setAttribute(Qt::WA_InputMethodEnabled, true);
    
    // 创建光标闪烁定时器
    cursorTimer = new QTimer(this);
    connect(cursorTimer, &QTimer::timeout, this, &TextInputWidget::blinkCursor);
    cursorTimer->start(500); // 500ms 闪烁间隔
    
    // 设置视图
    setRenderHint(QPainter::Antialiasing);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    
    // 添加测试文本
    GraphicsTextItem *testItem = new GraphicsTextItem();
    testItem->setText("点击此处开始输入...");
    testItem->setPos(100, 100);
    scene->addItem(testItem);
}

void TextInputWidget::mousePressEvent(QMouseEvent *event)
{
    QGraphicsView::mousePressEvent(event);

    // 将点击位置转换为场景坐标
    QPointF scenePos = mapToScene(event->pos());

    // 查找点击的文本项
    QGraphicsItem *item = scene->itemAt(scenePos, transform());
    GraphicsTextItem *textItem = qgraphicsitem_cast<GraphicsTextItem*>(item);

    if (textItem)
    {
        qDebug() << "Clicked on GraphicsTextItem, setting focus...";
        // 如果点击了文本项，激活它
        if (activeTextItem)
        {
            activeTextItem->setActive(false);
        }

        activeTextItem = textItem;
        activeTextItem->setActive(true);

        // 将焦点设置到文本项上，这样输入法才能正常工作
        scene->setFocusItem(activeTextItem);
        qDebug() << "Focus item set:" << scene->focusItem();

        // 计算点击位置在文本中的索引
        QPointF itemPos = textItem->mapFromScene(scenePos);
        int cursorPos = textItem->getCursorPositionAt(itemPos.x());
        activeTextItem->setCursorPosition(cursorPos);

        // 更新输入法查询
        updateMicroFocusPosition();

        // 确保光标可见
        updateCursor();
    }
    else if (activeTextItem)
    {
        // 点击空白处，取消激活当前文本项
        activeTextItem->setActive(false);
        activeTextItem = nullptr;
        scene->setFocusItem(nullptr);
        cursorTimer->stop();
    }
}

void TextInputWidget::keyPressEvent(QKeyEvent *event)
{
    // 只处理回车键创建新行，其他按键由 GraphicsTextItem 处理
    if (activeTextItem && (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter))
    {
        // 创建新文本项
        GraphicsTextItem *newItem = new GraphicsTextItem();
        newItem->setText("");
        newItem->setPos(activeTextItem->pos().x(),
                       activeTextItem->pos().y() + activeTextItem->boundingRect().height() + 5);
        scene->addItem(newItem);

        // 激活新文本项
        activeTextItem->setActive(false);
        activeTextItem = newItem;
        activeTextItem->setActive(true);
        activeTextItem->setCursorPosition(0);
        scene->setFocusItem(activeTextItem);

        event->accept();
    }
    else
    {
        QGraphicsView::keyPressEvent(event);
    }
}

// 输入法事件现在由 GraphicsTextItem 直接处理
void TextInputWidget::inputMethodEvent(QInputMethodEvent *event)
{
    // 转发给激活的文本项处理
    if (activeTextItem)
    {
        activeTextItem->inputMethodEvent(event);
    }
    else
    {
        event->ignore();
    }
}

QVariant TextInputWidget::inputMethodQuery(Qt::InputMethodQuery query) const
{
    if (!activeTextItem)
        return QGraphicsView::inputMethodQuery(query);
    
    switch (query)
    {
    case Qt::ImCursorRectangle:
    {
        // 返回光标的矩形区域（输入法候选框会显示在这个位置附近）
        QRectF cursorRect = activeTextItem->getCursorRect();
        QPointF scenePos = activeTextItem->mapToScene(cursorRect.topLeft());
        
        // 将场景坐标转换为视图坐标
        QPoint viewPos = mapFromScene(scenePos);
        
        // 将视图坐标转换为窗口坐标（相对于顶级窗口）
        QWidget *topLevel = window();
        QPoint windowPos = mapTo(topLevel, viewPos);
        
        // 创建矩形，保持大小不变
        QRect rect(windowPos, cursorRect.size().toSize());
        
        qDebug() << "InputMethodQuery ImCursorRectangle:";
        qDebug() << "  cursorRect:" << cursorRect;
        qDebug() << "  scenePos:" << scenePos;
        qDebug() << "  viewPos:" << viewPos;
        qDebug() << "  windowPos:" << windowPos;
        qDebug() << "  topLevel:" << topLevel;
        
        return rect;
    }
    
    case Qt::ImAnchorRectangle:
        // 锚点矩形（通常与光标矩形相同）
        return inputMethodQuery(Qt::ImCursorRectangle);
        
    case Qt::ImFont:
        // 返回当前字体
        return activeTextItem->getFont();
        
    case Qt::ImCursorPosition:
        // 返回光标位置
        return activeTextItem->getCursorPosition();
        
    case Qt::ImSurroundingText:
        // 返回光标周围的文本
        return activeTextItem->getText();
        
    case Qt::ImCurrentSelection:
        // 当前选择的文本（这里不实现选择）
        return QString();
        
    case Qt::ImMaximumTextLength:
        // 最大文本长度
        return 1000;
        
    case Qt::ImAnchorPosition:
        // 锚点位置（通常与光标位置相同）
        return activeTextItem->getCursorPosition();
        
    case Qt::ImHints:
        // 输入法提示
        return QVariant(static_cast<int>(Qt::ImhNone));
        
    case Qt::ImPreferredLanguage:
        // 首选语言
        return "zh-CN";
        
    default:
        return QGraphicsView::inputMethodQuery(query);
    }
}

void TextInputWidget::blinkCursor()
{
    if (activeTextItem && activeTextItem->isActive())
    {
        cursorVisible = !cursorVisible;
        activeTextItem->setCursorVisible(cursorVisible);
        activeTextItem->update();
    }
}

void TextInputWidget::updateCursor()
{
    if (activeTextItem)
    {
        cursorVisible = true;
        activeTextItem->setCursorVisible(cursorVisible);
        activeTextItem->update();
    }
}

void TextInputWidget::updateMicroFocusPosition()
{
    // 通知Qt更新输入法位置
    // 在Qt6中，我们需要确保输入法知道光标位置
    if (activeTextItem)
    {
        // 强制更新输入法查询
        QInputMethod *im = QGuiApplication::inputMethod();
        if (im)
        {
            im->update(Qt::ImQueryAll);
        }
    }
}