#include "view/TextEditorWidget.h"
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QLabel>
#include "core/Format.h"

TextEditorWidget::TextEditorWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_documentView(new DocumentView(this)),
      m_statusBar(new QStatusBar(this)),
      m_documentController(new DocumentController(this)),
      m_selectionController(new SelectionController(this)),
      m_inputController(new InputController(this)),
      m_document(nullptr)
{
    setupUI();
    setupConnections();
    
    // 创建默认文档
    m_document = new Document();
    Paragraph para;
    Format defaultFormat(QFont("Microsoft YaHei", 12));
    para.appendText("Hello, Math Editor!", defaultFormat);
    m_document->addParagraph(para);
    
    setDocument(m_document);
    
    // 设置初始选择
    Selection initialSelection({0, 0, 0}, {0, 0, 0});
    m_selectionController->setSelection(initialSelection);
    
    setFocusPolicy(Qt::StrongFocus);
    setAttribute(Qt::WA_InputMethodEnabled, true);
}

void TextEditorWidget::setDocument(Document *document) {
    m_document = document;
    m_documentController->setDocument(document);
    m_documentView->setDocument(document);
    
    if (document) {
        // 设置初始选择
        Selection initialSelection({0, 0, 0}, {0, 0, 0});
        m_selectionController->setSelection(initialSelection);
    }
}

Document *TextEditorWidget::document() const { return m_document; }

void TextEditorWidget::setupUI() {
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);
    
    m_layout->addWidget(m_documentView);
    m_layout->addWidget(m_statusBar);
    
    m_statusBar->setSizeGripEnabled(false);
    
    // 设置状态栏显示3行信息
    m_statusBar->showMessage("Ready");
}

void TextEditorWidget::setupConnections() {
    // 连接控制器和视图
    m_inputController->setDocumentController(m_documentController);
    m_inputController->setSelectionController(m_selectionController);
    m_inputController->setDocumentView(m_documentView);
    
    // 连接信号
    connect(m_documentView, &DocumentView::selectionChanged,
            this, &TextEditorWidget::onSelectionChanged);
    connect(m_documentView, &DocumentView::mousePositionChanged,
            this, &TextEditorWidget::onMousePositionChanged);
    
    connect(m_documentController, &DocumentController::documentChanged,
            m_documentView, &DocumentView::updateLayout);
    
    connect(m_selectionController, &SelectionController::selectionChanged,
            m_documentView, &DocumentView::setSelection);
    
    connect(m_selectionController, &SelectionController::selectionChanged,
            this, &TextEditorWidget::onSelectionChanged);
}

void TextEditorWidget::keyPressEvent(QKeyEvent *event) {
    m_inputController->handleKeyPress(event);
    if (!event->isAccepted()) {
        QWidget::keyPressEvent(event);
    }
}

void TextEditorWidget::inputMethodEvent(QInputMethodEvent *event) {
    m_inputController->handleInputMethodEvent(event);
}

QVariant TextEditorWidget::inputMethodQuery(Qt::InputMethodQuery query) const {
    if (m_documentView) {
        return m_documentView->inputMethodQueryPublic(query);
    }
    return QWidget::inputMethodQuery(query);
}

void TextEditorWidget::onSelectionChanged(const Selection &selection) {
    updateStatusBar();
}

void TextEditorWidget::onMousePositionChanged(const QPointF &scenePos, const QPoint &viewPos) {
    updateStatusBar();
}

void TextEditorWidget::updateStatusBar() {
    if (!m_documentView || !m_selectionController) return;
    
    Selection selection = m_selectionController->selection();
    Position pos = selection.start();
    
    QString statusText = QString("段落: %1, 项: %2, 偏移: %3")
                         .arg(pos.paragraph)
                         .arg(pos.item)
                         .arg(pos.offset);
    
    if (m_selectionController->hasSelection()) {
        statusText += " [选择中]";
    }
    
    m_statusBar->showMessage(statusText);
}