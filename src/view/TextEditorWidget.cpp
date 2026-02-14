// ============================================================================
// TextEditorWidget.cpp
// 文本编辑器部件实现文件
// 实现文本编辑器的界面布局、事件处理和状态管理
// ============================================================================

#include "view/TextEditorWidget.h"
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QLabel>
#include "core/Format.h"
#include "core/TextRun.h"
#include "util/PositionUtils.h"

/**
 * @brief 构造函数
 * 初始化文本编辑器部件，创建所有组件并设置默认文档
 * @param parent 父部件指针
 */
TextEditorWidget::TextEditorWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),
      m_documentView(new DocumentView(this)),
      m_statusBar(new QStatusBar(this)),
      m_statusLabel(nullptr),
      m_documentController(new DocumentController(this)),
      m_selectionController(new SelectionController(this)),
      m_inputController(new InputController(this)),
      m_document(nullptr),
      m_lastMouseScenePos(0,0),
      m_lastMouseViewPos(0,0)
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
    
    // 设置初始选择（光标位置在文档开头）
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
    
    // 创建两个标签：左侧显示主要信息，右侧专门显示鼠标信息
    m_statusLabel = new QLabel(this);
    m_statusLabel->setTextFormat(Qt::PlainText);
    m_statusLabel->setWordWrap(true);
    m_statusLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    
    m_mouseInfoLabel = new QLabel(this);
    m_mouseInfoLabel->setTextFormat(Qt::PlainText);
    m_mouseInfoLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    m_mouseInfoLabel->setMinimumWidth(200); // 确保有足够的空间显示鼠标信息
    
    // 添加标签到状态栏
    m_statusBar->addWidget(m_statusLabel, 1);   // 左侧占主要空间
    m_statusBar->addPermanentWidget(m_mouseInfoLabel); // 右侧永久显示鼠标信息
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
    // 同步到选区控制器
    if (m_selectionController) {
        m_selectionController->setSelection(selection);
    }

    m_documentView->ensureCursorVisible();
    updateStatusBar();
}

void TextEditorWidget::onMousePositionChanged(const QPointF &scenePos, const QPoint &viewPos) {
    m_lastMouseScenePos = scenePos;
    m_lastMouseViewPos = viewPos;
    updateStatusBar();
}



void TextEditorWidget::updateStatusBar() {
    if (!m_documentView || !m_documentView->cursor() || !m_document) return;

    Position pos = m_documentView->cursor()->position();
    int line = pos.paragraph + 1;

    // 列号处理（兼容公式项）
    QString colStr;
    if (pos.item < m_document->paragraph(pos.paragraph).itemCount()) {
        const auto &item = m_document->paragraph(pos.paragraph).itemAt(pos.item);
        if (item.type == Paragraph::TextRunItem) {
            colStr = QString::number(pos.offset + 1);
        } else {
            colStr = (pos.offset == 0) ? "公式前" : "公式后";
        }
    } else {
        colStr = "?";
    }

    int currentLineIdx = PositionUtils::currentParagraphCharIndex(pos, m_document);
    QPointF cursorPos = m_documentView->pointFromPosition(pos);

    // 获取左右字符信息
    CharInfo leftInfo = PositionUtils::getCharInfo(pos, m_document, m_documentView, true);
    CharInfo rightInfo = PositionUtils::getCharInfo(pos, m_document, m_documentView, false);

    // 构建主要状态信息（不含鼠标信息）
    QString statusText;
    statusText = QString("第%1行, 第%2列 | 当前行索引:%3 | 场景坐标: (%4, %5)\n")
                     .arg(line).arg(colStr).arg(currentLineIdx)
                     .arg(cursorPos.x(), 0, 'f', 2).arg(cursorPos.y(), 0, 'f', 2);

    // 第二行：左右字符信息
    QString leftCharStr = leftInfo.valid ? QString(leftInfo.ch) : "-";
    QString leftIdxStr = leftInfo.valid ? QString::number(leftInfo.currentLineIndex) : "-";
    QString rightCharStr = rightInfo.valid ? QString(rightInfo.ch) : "-";
    QString rightIdxStr = rightInfo.valid ? QString::number(rightInfo.currentLineIndex) : "-";

    statusText += QString("左字符索引:%1 字符:'%2' (坐标:%3,%4 宽:%5) | 右字符索引:%6 字符:'%7' (坐标:%8,%9 宽:%10)\n")
                       .arg(leftIdxStr).arg(leftCharStr)
                       .arg(leftInfo.pos.x(), 0, 'f', 2).arg(leftInfo.pos.y(), 0, 'f', 2).arg(leftInfo.width, 0, 'f', 2)
                       .arg(rightIdxStr).arg(rightCharStr)
                       .arg(rightInfo.pos.x(), 0, 'f', 2).arg(rightInfo.pos.y(), 0, 'f', 2).arg(rightInfo.width, 0, 'f', 2);

    // 第三行：选择状态和输入法组合
    QString thirdLine;
    if (m_selectionController && m_selectionController->hasSelection()) {
        thirdLine += "选择中 ";
    }
    if (m_inputController && !m_inputController->composingText().isEmpty()) {
        thirdLine += "输入法: " + m_inputController->composingText() + " ";
    }
    if (thirdLine.isEmpty()) thirdLine = "无";
    statusText += thirdLine + "\n";

    // 第四行：算法信息
    QString algoInfo = "光标定位:最近间隙 | 坐标计算:累积宽度";
    statusText += algoInfo;

    // 单独更新主要状态信息
    m_statusLabel->setText(statusText);
    
    // 单独更新鼠标信息（显示在最右边）
    QString mouseInfo = QString("鼠标: 场景(%1,%2) 视图(%3,%4)")
                             .arg(m_lastMouseScenePos.x(), 0, 'f', 2)
                             .arg(m_lastMouseScenePos.y(), 0, 'f', 2)
                             .arg(m_lastMouseViewPos.x())
                             .arg(m_lastMouseViewPos.y());
    m_mouseInfoLabel->setText(mouseInfo);
}