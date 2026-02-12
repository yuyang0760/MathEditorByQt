// ============================================================================
// TextEditorWidget.cpp
// 文本编辑器部件类的实现文件
// 主编辑器部件，集成了控制器和视图
// ============================================================================

#include "view/TextEditorWidget.h"
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QMouseEvent>

/**
 * @brief 构造函数
 * @param parent 父窗口部件
 */
TextEditorWidget::TextEditorWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_documentView(new DocumentView(this)),
      m_inputController(new InputController(this)),
      m_documentController(new DocumentController(this)),
      m_selectionController(new SelectionController(this))
{
    m_layout->addWidget(m_documentView);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled, true);
    setInputMethodHints(Qt::ImhMultiLine);
    
    // 设置控制器之间的关联
    m_inputController->setDocumentController(m_documentController);
    m_inputController->setSelectionController(m_selectionController);
    m_inputController->setDocumentView(m_documentView);
    
    // 连接选择变化信号，以便更新视图
    connect(m_selectionController, &SelectionController::selectionChanged, 
            this, &TextEditorWidget::onSelectionChanged);
    
    // 连接文档变化信号，以便更新视图
    connect(m_documentController, &DocumentController::documentChanged,
            this, &TextEditorWidget::onDocumentChanged);
    
    // 设置文档控制器的文档（暂时为空，将在setDocument时设置）
}

/**
 * @brief 文档变化处理
 * 文档发生变化时更新视图布局
 */
void TextEditorWidget::onDocumentChanged()
{
    // 文档发生变化时更新视图布局
    m_documentView->updateLayout();
}

/**
 * @brief 选择变化处理
 * @param selection 新的选择范围
 * 将选择更新到文档视图并确保光标可见
 */
void TextEditorWidget::onSelectionChanged(const Selection &selection)
{
    // 将选择更新到文档视图
    m_documentView->setSelection(selection);
    
    // 确保光标可见
    m_documentView->ensureCursorVisible();
}

/**
 * @brief 获取文档视图
 * @return 文档视图指针
 */
DocumentView *TextEditorWidget::documentView() const
{
    return m_documentView;
}

/**
 * @brief 获取文档
 * @return 文档指针
 */
Document *TextEditorWidget::document() const
{
    return m_documentView->document();
}

/**
 * @brief 设置文档
 * @param document 文档指针
 */
void TextEditorWidget::setDocument(Document *document)
{
    m_documentView->setDocument(document);
    m_documentController->setDocument(document);
    
    // 如果文档为空，创建一个默认的初始段落
    if (document && document->paragraphCount() == 0) {
        Paragraph paragraph;
        document->addParagraph(paragraph);
    }
}

/**
 * @brief 剪切功能
 * 实现剪切操作
 */
void TextEditorWidget::cut()
{
    // 实现剪切功能
    // 这里可以添加具体的剪切逻辑
}

/**
 * @brief 复制功能
 * 实现复制操作
 */
void TextEditorWidget::copy()
{
    // 实现复制功能
    // 这里可以添加具体的复制逻辑
}

/**
 * @brief 粘贴功能
 * 实现粘贴操作
 */
void TextEditorWidget::paste()
{
    // 实现粘贴功能
    // 这里可以添加具体的粘贴逻辑
}

/**
 * @brief 撤销功能
 * 实现撤销操作
 */
void TextEditorWidget::undo()
{
    // 实现撤销功能
    // 这里可以添加具体的撤销逻辑
}

/**
 * @brief 重做功能
 * 实现重做操作
 */
void TextEditorWidget::redo()
{
    // 实现重做功能
    // 这里可以添加具体的重做逻辑
}

/**
 * @brief 键盘按下事件处理
 * @param event 键盘事件
 */
void TextEditorWidget::keyPressEvent(QKeyEvent *event)
{
    // 首先尝试用输入控制器处理事件
    if (m_inputController) {
        m_inputController->handleKeyPress(event);
        
        // 如果事件已被处理，则接受事件并返回
        if (event->isAccepted()) {
            event->accept();
            return;
        }
    }
    
    // 处理特定的编辑快捷键
    if (event->matches(QKeySequence::Cut)) {
        cut();
        event->accept();
        return;
    } else if (event->matches(QKeySequence::Copy)) {
        copy();
        event->accept();
        return;
    } else if (event->matches(QKeySequence::Paste)) {
        paste();
        event->accept();
        return;
    } else if (event->matches(QKeySequence::Undo)) {
        undo();
        event->accept();
        return;
    } else if (event->matches(QKeySequence::Redo)) {
        redo();
        event->accept();
        return;
    }
    
    // 调用父类的处理
    QWidget::keyPressEvent(event);
}

/**
 * @brief 鼠标按下事件处理
 * @param event 鼠标事件
 */
void TextEditorWidget::mousePressEvent(QMouseEvent *event)
{
    QWidget::mousePressEvent(event);
    
    // 确保获得焦点，以便输入法能够正常工作
    setFocus();
}

/**
 * @brief 输入法事件处理
 * @param event 输入法事件
 */
void TextEditorWidget::inputMethodEvent(QInputMethodEvent *event)
{
    // 将输入法事件传递给输入控制器处理
    if (m_inputController) {
        m_inputController->handleInputMethodEvent(event);
    }
    
    // 调用父类的处理
    QWidget::inputMethodEvent(event);
}

/**
 * @brief 输入法查询
 * @param query 查询类型
 * @return 查询结果
 */
QVariant TextEditorWidget::inputMethodQuery(Qt::InputMethodQuery query) const
{
    // 委托给文档视图处理输入法查询
    if (m_documentView) {
        return m_documentView->inputMethodQueryPublic(query);
    }
    return QWidget::inputMethodQuery(query);
}
