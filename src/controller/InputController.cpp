#include "controller/InputController.h"
#include "controller/DocumentController.h"
#include "controller/SelectionController.h"
#include "view/DocumentView.h"

InputController::InputController(QObject *parent)
    : QObject(parent),
      m_documentController(nullptr),
      m_selectionController(nullptr),
      m_documentView(nullptr) {}

void InputController::setDocumentController(DocumentController *controller) {
    m_documentController = controller;
}

void InputController::setSelectionController(SelectionController *controller) {
    m_selectionController = controller;
}

void InputController::setDocumentView(DocumentView *view) {
    m_documentView = view;
}

void InputController::handleKeyPress(QKeyEvent *event) {
    if (!m_documentController || !m_selectionController) return;
    
    Selection selection = m_selectionController->selection(); // 实时获取
    
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 回车处理...
        m_documentController->insertParagraph(selection.start().paragraph + 1);
        
        // 移动光标到新段落
        Position newPos;
        newPos.paragraph = selection.start().paragraph + 1;
        newPos.item = 0;
        newPos.offset = 0;
        Selection newSelection(newPos, newPos);
        m_selectionController->setSelection(newSelection);
        event->accept();
    }
    else if (!event->text().isEmpty() && event->text()[0].isPrint()) {
        QString text = event->text();
        if (m_selectionController->hasSelection()) {
            m_documentController->replaceText(selection, text);
            // 新光标位置 = 起始位置 + 文本长度
            Position newPos = selection.start();
            newPos.offset += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        } else {
            m_documentController->insertText(selection.start(), text);
            Position newPos = selection.start();
            newPos.offset += text.length();
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

void InputController::handleInputMethodEvent(QInputMethodEvent *event) {
    if (!m_documentController || !m_selectionController) return;
    
    Selection selection = m_selectionController->selection();
    
    if (!event->commitString().isEmpty()) {
        QString text = event->commitString();
        // 插入/替换逻辑
        if (m_selectionController->hasSelection()) {
            m_documentController->replaceText(selection, text);
            Position newPos = selection.start();
            newPos.offset += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        } else {
            m_documentController->insertText(selection.start(), text);
            Position newPos = selection.start();
            newPos.offset += text.length();
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
            // 显示组合文本的逻辑
        }
    } else {
        m_composingText.clear();
        if (m_documentView) {
            // 隐藏组合文本的逻辑
        }
    }
    
    event->accept();
}