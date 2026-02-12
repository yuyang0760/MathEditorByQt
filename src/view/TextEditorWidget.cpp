#include "view/TextEditorWidget.h"
#include <QKeyEvent>

TextEditorWidget::TextEditorWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_documentView(new DocumentView(this))
{
    m_layout->addWidget(m_documentView);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    setFocusPolicy(Qt::StrongFocus);
}

DocumentView *TextEditorWidget::documentView() const
{
    return m_documentView;
}

Document *TextEditorWidget::document() const
{
    return m_documentView->document();
}

void TextEditorWidget::setDocument(Document *document)
{
    m_documentView->setDocument(document);
}

void TextEditorWidget::cut()
{
    // 实现剪切功能
    // 这里可以添加具体的剪切逻辑
}

void TextEditorWidget::copy()
{
    // 实现复制功能
    // 这里可以添加具体的复制逻辑
}

void TextEditorWidget::paste()
{
    // 实现粘贴功能
    // 这里可以添加具体的粘贴逻辑
}

void TextEditorWidget::undo()
{
    // 实现撤销功能
    // 这里可以添加具体的撤销逻辑
}

void TextEditorWidget::redo()
{
    // 实现重做功能
    // 这里可以添加具体的重做逻辑
}

void TextEditorWidget::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    
    // 处理键盘事件，如Ctrl+C、Ctrl+V等
    // 这里可以添加具体的键盘事件处理逻辑
}
