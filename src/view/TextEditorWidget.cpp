// ============================================================================
// TextEditorWidget.cpp
// 文本编辑器部件实现文件
// 实现文本编辑器的界面布局、事件处理和状态管理
// ============================================================================

#include "view/TextEditorWidget.h"
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QLabel>
#include <QToolBar>
#include <QComboBox>
#include <QFontComboBox>
#include <QPushButton>
#include <QAction>
#include <QColorDialog>
#include "core/Format.h"
#include "core/TextRun.h"
#include "util/PositionUtils.h"
#include "core/StyleManager.h"

/**
 * @brief 构造函数
 * 初始化文本编辑器部件，创建所有组件并设置默认文档
 * @param parent 父部件指针
 */
TextEditorWidget::TextEditorWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(nullptr), // 不再使用QVBoxLayout，将在setupUI中创建QHBoxLayout
      m_toolBar(new QToolBar(this)),                 // 新增
      m_styleCombo(new QComboBox(this)),                   // 新增
      m_formatToolBar(new QToolBar(this)),           // 新增：格式工具栏
      m_fontCombo(new QFontComboBox(this)),          // 新增：字体选择框
      m_fontSizeCombo(new QComboBox(this)),          // 新增：字号选择框
      m_boldAction(new QAction("B", this)),          // 新增：粗体按钮
      m_italicAction(new QAction("I", this)),        // 新增：斜体按钮
      m_underlineAction(new QAction("U", this)),     // 新增：下划线按钮
      m_colorButton(new QPushButton("颜色", this)),  // 新增：颜色按钮
      m_documentView(new DocumentView(this)),
      m_documentController(new DocumentController(this)),
      m_selectionController(new SelectionController(this)),
      m_inputController(new InputController(this)),
      m_document(nullptr),
      m_updatingFromSelection(false)
{
    setupUI();
    setupConnections();
    
    // 创建默认文档
    m_document = new Document();
    
    // 第一行：英语
    Paragraph para1;
    Format defaultFormat(QFont("Microsoft YaHei", 12));
    para1.appendText("Hello, Math Editor!", defaultFormat);
    m_document->addParagraph(para1);
    
    // 第二行：中文
    Paragraph para2;
    para2.appendText("欢迎使用数学编辑器！", defaultFormat);
    m_document->addParagraph(para2);
    
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
    // 设置窗口大小
    resize(1000, 800);
    
    // 创建水平分割布局
    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    
    // 左侧容器（占全部空间）
    QWidget *leftWidget = new QWidget(this);
    QVBoxLayout *leftLayout = new QVBoxLayout(leftWidget);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(0);
    
    // 样式工具栏
    m_toolBar->addWidget(new QLabel("样式:"));
    m_toolBar->addWidget(m_styleCombo);
    m_styleCombo->setEditable(false);
    
    leftLayout->addWidget(m_toolBar);

    // 直接格式工具栏
    m_formatToolBar->addWidget(new QLabel("字体:"));
    m_formatToolBar->addWidget(m_fontCombo);
    
    m_formatToolBar->addWidget(new QLabel("字号:"));
    m_fontSizeCombo->setEditable(true);
    m_fontSizeCombo->addItems(QStringList() << "8" << "9" << "10" << "11" << "12" << "14" << "16" << "18" << "20" << "22" << "24" << "26" << "28" << "36" << "48" << "72");
    m_fontSizeCombo->setCurrentText("18");
    m_fontSizeCombo->setFixedWidth(80);  // 设置字号组合框宽度
    m_formatToolBar->addWidget(m_fontSizeCombo);
    
    m_boldAction->setCheckable(true);
    m_boldAction->setShortcut(QKeySequence::Bold);
    m_formatToolBar->addAction(m_boldAction);
    
    m_italicAction->setCheckable(true);
    m_italicAction->setShortcut(QKeySequence::Italic);
    m_formatToolBar->addAction(m_italicAction);
    
    m_underlineAction->setCheckable(true);
    m_underlineAction->setShortcut(QKeySequence::Underline);
    m_formatToolBar->addAction(m_underlineAction);
    
    m_formatToolBar->addWidget(m_colorButton);
    
    leftLayout->addWidget(m_formatToolBar);

    leftLayout->addWidget(m_documentView, 1);
    
    // 将左右容器添加到主布局
    mainLayout->addWidget(leftWidget, 1);  // 左侧占全部空间
    
    // 设置主布局
    setLayout(mainLayout);
}

void TextEditorWidget::setupConnections() {
    // 连接控制器和视图
    m_inputController->setDocumentController(m_documentController);
    m_inputController->setSelectionController(m_selectionController);
    m_inputController->setDocumentView(m_documentView);
    
    // 连接信号
    connect(m_documentView, &DocumentView::selectionChanged,
            this, &TextEditorWidget::onSelectionChanged);

    // 样式相关
    connect(m_styleCombo, QOverload<int>::of(&QComboBox::activated),
            this, &TextEditorWidget::onStyleSelected);
    connect(StyleManager::instance(), &StyleManager::stylesChanged,
            this, &TextEditorWidget::onStylesChanged);

    // 直接格式相关
    connect(m_fontCombo, &QFontComboBox::currentFontChanged,
            this, &TextEditorWidget::onFontFamilyChanged);
    connect(m_fontSizeCombo, &QComboBox::currentTextChanged,
            this, &TextEditorWidget::onFontSizeChanged);
    connect(m_boldAction, &QAction::toggled,
            this, &TextEditorWidget::onBoldToggled);
    connect(m_italicAction, &QAction::toggled,
            this, &TextEditorWidget::onItalicToggled);
    connect(m_underlineAction, &QAction::toggled,
            this, &TextEditorWidget::onUnderlineToggled);
    connect(m_colorButton, &QPushButton::clicked,
            this, &TextEditorWidget::onColorClicked);

    updateStyleCombo(); // 初始化样式下拉框
    
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
/*
 * @brief 处理选区变化事件
 * @param selection 新的选区
 * 
 * 当用户通过鼠标或键盘操作改变选区时，会触发此事件。
 * 该方法负责更新选区控制器、确保光标可见、更新状态条、
 * 并根据选区是否为空来更新格式工具栏状态。
*/
void TextEditorWidget::onSelectionChanged(const Selection &selection) {
    // 同步到选区控制器
    if (m_selectionController) {
        m_selectionController->setSelection(selection);
    }

    m_documentView->ensureCursorVisible();
    
    // 更新格式工具栏状态
    if (selection.isEmpty()) {
        // 无选择时：显示光标左侧字符的样式为当前直接格式
        Format charFormat = getCharacterFormatBeforeCursor();
        // 检查字体是否有效（通过字体族名是否为空来判断）
        if (!charFormat.font().family().isEmpty()) {
            m_documentController->setCurrentDirectFormat(charFormat);
        }
        
        // 显示当前直接格式
        Format currentFormat = m_documentController->currentDirectFormat();
        updateFormatToolbar(currentFormat);
    } else {
        // 规则2：如果有选择，显示最后一个选中字符的样式
        m_updatingFromSelection = true; // 标记正在从选择更新工具栏
        Format selectionFormat = getSelectionFormat(selection);

        // 将获取到的格式信息保存到成员变量，用于状态栏显示
        m_lastSelectionFormat = selectionFormat;
        
        // 使用updateFormatToolbar方法来正确更新工具栏，避免触发信号
        updateFormatToolbar(selectionFormat);
        
        m_updatingFromSelection = false; // 清除标记
    }
}
/* 
 * @brief 获取选择范围内文本的有效格式
 * @param selection 选择范围
 * @return 选择范围内文本的有效格式
*/
Format TextEditorWidget::getSelectionFormat(const Selection &selection) {
    // 获取选择范围内文本的有效格式
    if (!m_document || selection.isEmpty()) {
        return Format();
    }
    
    Position start = selection.normalizedStart();
    Position end = selection.normalizedEnd();
    
    // 根据规则2：获取最后一个选中字符的样式
    // 最后一个字符是选中字符的最后一个字符（selection.end()）
    Position lastCharPos = selection.end();

    // 如果是多段选择，返回最后一个选中字符所在段落的格式
    Paragraph &para = m_document->paragraph(lastCharPos.paragraphIndex);

    // 检查最后一个字符所在项是否有效
    if (lastCharPos.itemIndex < para.itemCount()) {
        auto &item = para.itemAt(lastCharPos.itemIndex);
        // 如果是文本项，获取其有效格式（包含字符格式和段落格式的合并结果）
        if (item.type == Paragraph::TextRunItem) {
            TextRun run = item.data.value<TextRun>();
            StyleManager *styleMgr = StyleManager::instance();
            // 返回文本运行的有效格式（包含字符直接格式和样式继承格式）
            return run.effectiveFormat(styleMgr);
        }
    }
    
    return Format();
}
/* 
 * @brief 获取光标前字符的格式（现在主要用于调试或未来扩展）
 * @return 光标前字符的格式
*/
Format TextEditorWidget::getCharacterFormatBeforeCursor() {
    // 获取光标前字符的格式（现在主要用于调试或未来扩展）
    if (!m_document) {
        return Format();
    }
    
    // 获取当前光标位置
    Selection currentSel = m_selectionController->selection();
    if (currentSel.isEmpty()) {
        // 如果没有选择，获取光标前一个字符的位置
        Position cursorPos = currentSel.start();
        if (cursorPos.paragraphIndex >= 0 && cursorPos.paragraphIndex < m_document->paragraphCount()) {
            Paragraph &para = m_document->paragraph(cursorPos.paragraphIndex);
            if (cursorPos.itemIndex >= 0 && cursorPos.itemIndex < para.itemCount()) {
                // 获取光标前一个字符的格式
                auto &item = para.itemAt(cursorPos.itemIndex);
                if (item.type == Paragraph::TextRunItem) {
                    TextRun run = item.data.value<TextRun>();
                    StyleManager *styleMgr = StyleManager::instance();
                    return run.effectiveFormat(styleMgr);
                }
            }
        }
    }
    
    return Format();
}

QString TextEditorWidget::getSelectedText(const Selection &selection) {
    // 获取选中文字的内容
    if (!m_document || selection.isEmpty()) {
        return QString();
    }
    
    Position start = selection.normalizedStart();
    Position end = selection.normalizedEnd();
    QString result;
    
    // 单段落选择
    if (start.paragraphIndex == end.paragraphIndex) {
        Paragraph &para = m_document->paragraph(start.paragraphIndex);
        
        // 遍历选中范围内的项
        for (int itemIndex = start.itemIndex; itemIndex <= end.itemIndex; ++itemIndex) {
            if (itemIndex < para.itemCount()) {
                auto &item = para.itemAt(itemIndex);
                if (item.type == Paragraph::TextRunItem) {
                    TextRun run = item.data.value<TextRun>();
                    QString text = run.text();
                    
                    // 处理当前项的选中部分
                    if (itemIndex == start.itemIndex && itemIndex == end.itemIndex) {
                        // 同一项内的部分选择
                        result += text.mid(start.offset, end.offset - start.offset);
                    } else if (itemIndex == start.itemIndex) {
                        // 起始项：从offset到结尾
                        result += text.mid(start.offset);
                    } else if (itemIndex == end.itemIndex) {
                        // 结束项：从开头到offset
                        result += text.left(end.offset);
                    } else {
                        // 中间完整项
                        result += text;
                    }
                }
            }
        }
    }
    
    return result;
}



void TextEditorWidget::updateStyleCombo() {
    m_styleCombo->clear();
    QList<Style> styles = StyleManager::instance()->allStyles();
    for (const Style &style : styles) {
        m_styleCombo->addItem(style.name(), style.id());
    }
}

void TextEditorWidget::onStyleSelected(int index) {
    QString styleId = m_styleCombo->itemData(index).toString();
    Selection sel = m_selectionController->selection();
    m_documentController->applyStyle(sel, styleId);
}

void TextEditorWidget::onStylesChanged() {
    updateStyleCombo();
}

/**
 * @brief 更新格式工具栏
 * @param format 当前选择格式
 */
void TextEditorWidget::updateFormatToolbar(const Format &format) {

    // 更新字体
    m_fontCombo->setCurrentFont(format.font());
    
    // 更新字号
    int fontSize = format.font().pointSize();
    if (fontSize > 0) {
        m_fontSizeCombo->setCurrentText(QString::number(fontSize));
    }
    
    // 更新粗体、斜体、下划线
    m_boldAction->setChecked(format.bold());
    m_italicAction->setChecked(format.italic());
    m_underlineAction->setChecked(format.underline());
    
    // 更新颜色按钮样式
    QColor color = format.color();
    if (color.isValid()) {
        QString style = QString("QPushButton { background-color: %1; color: %2; }")
                            .arg(color.name())
                            .arg(color.lightness() > 128 ? "black" : "white");
        m_colorButton->setStyleSheet(style);
    } else {
        m_colorButton->setStyleSheet("");
    }
}

void TextEditorWidget::onFontFamilyChanged(const QFont &font) {
    // 先获取当前直接格式并修改
    Format currentFormat = m_documentController->currentDirectFormat();
    QFont newFont = currentFormat.font();
    newFont.setFamily(font.family());
    currentFormat.setFont(newFont);
    
    // 设置新的直接格式
    m_documentController->setCurrentDirectFormat(currentFormat);
    
    // 如果有选中文字，应用到选中文字
    Selection sel = m_selectionController->selection();
    if (!sel.isEmpty()) {
        m_documentController->applyDirectFormat(sel, currentFormat);
    }
}

void TextEditorWidget::onFontSizeChanged(const QString &size) {
    bool ok;
    int fontSize = size.toInt(&ok);
    if (ok && fontSize > 0) {
        // 先获取当前直接格式并修改
        Format currentFormat = m_documentController->currentDirectFormat();
        QFont newFont = currentFormat.font();
        newFont.setPointSize(fontSize);
        currentFormat.setFont(newFont);
        
        // 设置新的直接格式
        m_documentController->setCurrentDirectFormat(currentFormat);
        
        // 如果有选中文字，应用到选中文字
        Selection sel = m_selectionController->selection();
        if (!sel.isEmpty()) {
            m_documentController->applyDirectFormat(sel, currentFormat);
        }
    }
}

void TextEditorWidget::onBoldToggled(bool checked) {
    // 先获取当前直接格式并修改
    Format currentFormat = m_documentController->currentDirectFormat();
    currentFormat.setBold(checked);
    
    // 设置新的直接格式
    m_documentController->setCurrentDirectFormat(currentFormat);
    
    // 如果有选中文字，应用到选中文字
    Selection sel = m_selectionController->selection();
    if (!sel.isEmpty()) {
        m_documentController->applyDirectFormat(sel, currentFormat);
    }
}

void TextEditorWidget::onItalicToggled(bool checked) {
    // 先获取当前直接格式并修改
    Format currentFormat = m_documentController->currentDirectFormat();
    currentFormat.setItalic(checked);
    
    // 设置新的直接格式
    m_documentController->setCurrentDirectFormat(currentFormat);
    
    // 如果有选中文字，应用到选中文字
    Selection sel = m_selectionController->selection();
    if (!sel.isEmpty()) {
        m_documentController->applyDirectFormat(sel, currentFormat);
    }
}

void TextEditorWidget::onUnderlineToggled(bool checked) {
    // 先获取当前直接格式并修改
    Format currentFormat = m_documentController->currentDirectFormat();
    currentFormat.setUnderline(checked);
    
    // 设置新的直接格式
    m_documentController->setCurrentDirectFormat(currentFormat);
    
    // 如果有选中文字，应用到选中文字
    Selection sel = m_selectionController->selection();
    if (!sel.isEmpty()) {
        m_documentController->applyDirectFormat(sel, currentFormat);
    }
}

void TextEditorWidget::onColorClicked() {
    Format currentFormat = m_documentController->currentDirectFormat();
    QColor currentColor = currentFormat.color();
    
    QColorDialog dialog(currentColor.isValid() ? currentColor : Qt::black, this);
    if (dialog.exec() == QDialog::Accepted) {
        QColor selectedColor = dialog.selectedColor();
        
        // 先获取当前直接格式并修改
        currentFormat.setColor(selectedColor);
        
        // 设置新的直接格式
        m_documentController->setCurrentDirectFormat(currentFormat);
        
        // 如果有选中文字，应用到选中文字
        Selection sel = m_selectionController->selection();
        if (!sel.isEmpty()) {
            m_documentController->applyDirectFormat(sel, currentFormat);
        }
    }
}