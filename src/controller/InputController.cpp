#include "controller/InputController.h"
#include <QKeyEvent>

InputController::InputController(QObject *parent)
    : QObject(parent),
      m_documentController(nullptr),
      m_selectionController(nullptr)
{
}

void InputController::setDocumentController(DocumentController *controller)
{
    m_documentController = controller;
}

void InputController::setSelectionController(SelectionController *controller)
{
    m_selectionController = controller;
}

void InputController::handleKeyPress(QKeyEvent *event)
{
    if (!m_documentController || !m_selectionController)
        return;
    
    Selection selection = m_selectionController->selection();
    
    // 处理不同类型的按键
    if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)
    {
        if (m_selectionController->hasSelection())
        {
            // 删除选中的文本
            m_documentController->deleteText(selection);
            
            // 折叠选择到起始位置
            Selection newSelection(selection.start(), selection.start());
            m_selectionController->setSelection(newSelection);
        }
        else
        {
            // 删除光标前或后的一个字符
            // 这里可以添加具体的删除逻辑
        }
    }
    else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter)
    {
        // 处理回车键，插入新段落
        m_documentController->insertParagraph(selection.start().paragraph + 1);
        
        // 移动光标到新段落
        Selection::Position newPos;
        newPos.paragraph = selection.start().paragraph + 1;
        newPos.position = 0;
        Selection newSelection(newPos, newPos);
        m_selectionController->setSelection(newSelection);
    }
    else if (event->key() >= Qt::Key_Space && event->key() <= Qt::Key_AsciiTilde)
    {
        // 处理可打印字符
        QString text = event->text();
        
        if (m_selectionController->hasSelection())
        {
            // 替换选中的文本
            m_documentController->replaceText(selection, text);
            
            // 移动光标到文本末尾
            Selection::Position newPos = selection.start();
            newPos.position += text.length();
            Selection newSelection(newPos, newPos);
            m_selectionController->setSelection(newSelection);
        }
        else
        {
            // 插入文本
            m_documentController->insertText(selection.start(), text);
            
            // 移动光标到文本末尾
            Selection::Position newPos = selection.start();
            newPos.position += text.length();
            Selection newSelection(newPos, newPos);
            m_selectionController->setSelection(newSelection);
        }
    }
    else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        // 处理方向键，移动光标
        // 这里可以添加具体的光标移动逻辑
    }
}

void InputController::handleInputMethodEvent(QInputMethodEvent *event)
{
    // 处理输入法事件
    // 这里可以添加具体的输入法处理逻辑
}

void InputController::handleMousePress(const QPoint &point)
{
    // 处理鼠标按下事件
    // 这里可以添加具体的鼠标事件处理逻辑
}

void InputController::handleMouseMove(const QPoint &point)
{
    // 处理鼠标移动事件
    // 这里可以添加具体的鼠标事件处理逻辑
}

void InputController::handleMouseRelease(const QPoint &point)
{
    // 处理鼠标释放事件
    // 这里可以添加具体的鼠标事件处理逻辑
}

void InputController::updateInputMethod()
{
    // 更新输入法状态
    // 这里可以添加具体的输入法更新逻辑
}
