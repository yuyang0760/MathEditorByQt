// ============================================================================
// InputController.cpp
// 输入控制器类的实现文件
// 负责处理用户输入事件，如键盘输入、鼠标事件和输入法事件
// ============================================================================

#include "controller/InputController.h"
#include "view/DocumentView.h"
#include <QKeyEvent>

/**
 * @brief 构造函数
 * @param parent 父对象
 */
InputController::InputController(QObject *parent)
    : QObject(parent),
      m_documentController(nullptr),
      m_selectionController(nullptr),
      m_documentView(nullptr)
{
}

/**
 * @brief 设置文档控制器
 * @param controller 文档控制器
 */
void InputController::setDocumentController(DocumentController *controller)
{
    m_documentController = controller;
}

/**
 * @brief 设置选择控制器
 * @param controller 选择控制器
 */
void InputController::setSelectionController(SelectionController *controller)
{
    m_selectionController = controller;
}

/**
 * @brief 设置文档视图
 * @param view 文档视图
 */
void InputController::setDocumentView(DocumentView *view)
{
    m_documentView = view;
}

/**
 * @brief 处理键盘按下事件
 * @param event 键盘事件
 */
void InputController::handleKeyPress(QKeyEvent *event)
{
    if (!m_documentController || !m_selectionController) return;
    
    Selection selection = m_selectionController->selection(); // 实时获取
    
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 回车处理...
        m_documentController->insertParagraph(selection.start().paragraph + 1);
        
        // 移动光标到新段落
        Selection::Position newPos;
        newPos.paragraph = selection.start().paragraph + 1;
        newPos.position = 0;
        Selection newSelection(newPos, newPos);
        m_selectionController->setSelection(newSelection);
        event->accept();
    }
    else if (!event->text().isEmpty() && event->text()[0].isPrint()) {
        QString text = event->text();
        if (m_selectionController->hasSelection()) {
            m_documentController->replaceText(selection, text);
            // 新光标位置 = 起始位置 + 文本长度
            Selection::Position newPos = selection.start();
            newPos.position += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        } else {
            m_documentController->insertText(selection.start(), text);
            Selection::Position newPos = selection.start();
            newPos.position += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        }
        if (m_documentView) {
            m_documentView->updateLayout();
            m_documentView->ensureCursorVisible();
        }
        event->accept();
    }
    // ... 其他按键
    else if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)
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
        event->accept();
    }
    else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        // 处理方向键，移动光标
        // 这里可以添加具体的光标移动逻辑
        event->accept();
    }
}

/**
 * @brief 处理输入法事件
 * @param event 输入法事件
 */
void InputController::handleInputMethodEvent(QInputMethodEvent *event)
{
    if (!m_documentController || !m_selectionController) return;
    
    Selection selection = m_selectionController->selection();
    
    if (!event->commitString().isEmpty()) {
        QString text = event->commitString();
        // 插入/替换逻辑
        if (m_selectionController->hasSelection()) {
            m_documentController->replaceText(selection, text);
            Selection::Position newPos = selection.start();
            newPos.position += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        } else {
            m_documentController->insertText(selection.start(), text);
            Selection::Position newPos = selection.start();
            newPos.position += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        }
        if (m_documentView) {
            m_documentView->updateLayout();
            m_documentView->ensureCursorVisible();
        }
        m_composingText.clear();
    }
    
    // 组合文本显示
    if (!event->preeditString().isEmpty()) {
        m_composingText = event->preeditString();
        if (m_documentView) {
            m_documentView->showComposingText(m_composingText);
        }
    } else {
        m_composingText.clear();
        if (m_documentView) {
            m_documentView->hideComposingText();
        }
    }
    
    event->accept();
}

/**
 * @brief 处理鼠标按下事件
 * @param point 鼠标位置
 */
void InputController::handleMousePress(const QPoint &point)
{
    // 处理鼠标按下事件
    // 这里可以添加具体的鼠标事件处理逻辑
}

/**
 * @brief 处理鼠标移动事件
 * @param point 鼠标位置
 */
void InputController::handleMouseMove(const QPoint &point)
{
    // 处理鼠标移动事件
    // 这里可以添加具体的鼠标事件处理逻辑
}

/**
 * @brief 处理鼠标释放事件
 * @param point 鼠标位置
 */
void InputController::handleMouseRelease(const QPoint &point)
{
    // 处理鼠标释放事件
    // 这里可以添加具体的鼠标事件处理逻辑
}

/**
 * @brief 更新输入法状态
 */
void InputController::updateInputMethod()
{
    // 更新输入法状态
    // 这里可以添加具体的输入法更新逻辑
}
