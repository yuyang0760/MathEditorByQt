// ============================================================================
// InputController.cpp
// 输入控制器实现文件
// 处理键盘输入和输入法事件，转换为文档编辑操作
// ============================================================================

// ============================================================================
// InputController.cpp
// 输入控制器实现文件
// 处理键盘输入和输入法事件，转换为文档编辑操作
// ============================================================================

#include "controller/InputController.h"
#include "controller/DocumentController.h"
#include "controller/SelectionController.h"
#include "view/DocumentView.h"

/**
 * @brief 构造函数
 * 初始化输入控制器，设置父对象和默认指针
 * @param parent 父对象指针
 */
/**
 * @brief 构造函数
 * 初始化输入控制器，设置父对象和默认指针
 * @param parent 父对象指针
 */
InputController::InputController(QObject *parent)
    : QObject(parent),
      m_documentController(nullptr),
      m_selectionController(nullptr),
      m_documentView(nullptr) {}

/**
 * @brief 设置文档控制器
 * @param controller 文档控制器指针
 */
void InputController::setDocumentController(DocumentController *controller) {
    m_documentController = controller;
}

/**
 * @brief 设置选择控制器
 * @param controller 选择控制器指针
 */
void InputController::setSelectionController(SelectionController *controller) {
    m_selectionController = controller;
}

/**
 * @brief 设置文档视图
 * @param view 文档视图指针
 */
void InputController::setDocumentView(DocumentView *view) {
    m_documentView = view;
}

/**
 * @brief 处理键盘按下事件
 * 
 * 该方法处理用户的键盘输入，包括：
 * 1. 回车键：插入新段落并移动光标
 * 2. 可打印字符：插入或替换文本
 * 3. 退格/删除键：删除选中内容或单个字符
 * 4. 方向键：移动光标位置
 * 
 * @param event 键盘事件对象
 */
void InputController::handleKeyPress(QKeyEvent *event) {
    // 检查必要的控制器是否存在
    if (!m_documentController || !m_selectionController) return;
    
    // 实时获取当前选择区域
    Selection selection = m_selectionController->selection();
    
    // 处理回车键
    if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        // 在当前段落之后插入新段落
        m_documentController->insertParagraph(selection.start().paragraph + 1);
        
        // 移动光标到新段落的起始位置
        Position newPos;
        newPos.paragraph = selection.start().paragraph + 1; // 新段落索引
        newPos.item = 0;                                   // 第一个项
        newPos.offset = 0;                                 // 项内偏移为0
        Selection newSelection(newPos, newPos);            // 创建新的选择区域
        m_selectionController->setSelection(newSelection);  // 更新选择
        event->accept();                                   // 接受事件
    }
    // 处理可打印字符
    else if (!event->text().isEmpty() && event->text()[0].isPrint()) {
        QString text = event->text();                      // 获取输入文本
        
        if (m_selectionController->hasSelection()) {
            // 有选中内容：替换选中的文本
            m_documentController->replaceText(selection, text);
            // 计算新光标位置：起始位置 + 文本长度
            Position newPos = selection.start();
            newPos.offset += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        } else {
            // 无选中内容：在光标位置插入文本
            m_documentController->insertText(selection.start(), text);
            // 计算新光标位置：起始位置 + 文本长度
            Position newPos = selection.start();
            newPos.offset += text.length();
            m_selectionController->setSelection(Selection(newPos, newPos));
        }
        
        // 如果有文档视图，更新布局并确保光标可见
        if (m_documentView) {
            m_documentView->updateLayout();               // 重建场景
            m_documentView->ensureCursorVisible();        // 确保光标在可视区域
        }
        event->accept();                                   // 接受事件
    }
    // 处理退格键和删除键
    else if (event->key() == Qt::Key_Backspace || event->key() == Qt::Key_Delete)
    {
        if (m_selectionController->hasSelection())
        {
            // 有选中内容：删除选中的文本
            m_documentController->deleteText(selection);
            
            // 折叠选择到起始位置
            Selection newSelection(selection.start(), selection.start());
            m_selectionController->setSelection(newSelection);
        }
        else
        {
            // 无选中内容：删除光标前或后的一个字符
            // 这里可以添加具体的删除逻辑
        }
        event->accept();                                   // 接受事件
    }
    // 处理方向键
    else if (event->key() == Qt::Key_Left || event->key() == Qt::Key_Right ||
             event->key() == Qt::Key_Up || event->key() == Qt::Key_Down)
    {
        // 处理方向键，移动光标
        // 这里可以添加具体的光标移动逻辑
        event->accept();                                   // 接受事件
    }
}

/**
 * @brief 处理输入法事件
 * 
 * 该方法处理输入法事件，支持中文、日文等复杂输入：
 * 1. 提交文本：插入或替换文本
 * 2. 组合文本：显示输入法候选文本（待实现）
 * 
 * @param event 输入法事件对象
 */
/**
 * @brief 处理输入法事件
 * 
 * 该方法处理输入法事件，支持中文、日文等复杂输入：
 * 1. 提交文本：插入或替换文本
 * 2. 组合文本：显示输入法候选文本（待实现）
 * 
 * @param event 输入法事件对象
 */
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