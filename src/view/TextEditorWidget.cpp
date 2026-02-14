// ============================================================================
// TextEditorWidget.cpp
// 文本编辑器部件类的实现文件
// 主编辑器部件，集成了控制器和视图
// 
// 该类是整个数学公式编辑器的核心UI组件，负责：
// 1. 整合文档视图、输入控制器、文档控制器和选择控制器
// 2. 处理用户输入事件（键盘、鼠标、输入法）
// 3. 提供标准的编辑操作接口（剪切、复制、粘贴等）
// 4. 协调各组件间的消息传递和状态同步
// ============================================================================

#include "view/TextEditorWidget.h"
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QMouseEvent>

/**
 * @brief 构造函数
 * @param parent 父窗口部件
 * 
 * 初始化文本编辑器的所有核心组件：
 * - 创建垂直布局管理器用于组织界面元素
 * - 初始化文档视图组件，负责文档内容的渲染显示
 * - 初始化输入控制器，处理各种输入事件
 * - 初始化文档控制器，管理文档的核心操作
 * - 初始化选择控制器，管理文本选择状态
 * 
 * 设置组件间的依赖关系和信号连接，建立完整的数据流。
 */
TextEditorWidget::TextEditorWidget(QWidget *parent)
    : QWidget(parent),
      m_layout(new QVBoxLayout(this)),           // 垂直布局管理器
      m_documentView(new DocumentView(this)),    // 文档视图组件
      m_inputController(new InputController(this)),     // 输入事件控制器
      m_documentController(new DocumentController(this)), // 文档操作控制器
      m_selectionController(new SelectionController(this)), // 选择状态控制器
      m_statusBar(new QStatusBar(this)),         // 状态栏
      m_cursorPositionLabel(new QLabel("光标位置: 第1行, 第1列")) // 光标位置标签
{
    // 将文档视图添加到布局中
    m_layout->addWidget(m_documentView);
    // 添加状态栏
    m_layout->addWidget(m_statusBar);
    m_layout->setContentsMargins(0, 0, 0, 0);
    setLayout(m_layout);
    
    // 设置状态栏
    m_statusBar->addWidget(m_cursorPositionLabel);
    
    // 设置焦点策略和输入法支持
    setFocusPolicy(Qt::StrongFocus);                    // 强焦点策略，可接收键盘输入
    setAttribute(Qt::WA_InputMethodEnabled, true);      // 启用输入法支持
    setInputMethodHints(Qt::ImhMultiLine);              // 设置多行文本输入提示
    
    // 建立控制器间的依赖关系
    m_inputController->setDocumentController(m_documentController);    // 输入控制器需要文档控制器
    m_inputController->setSelectionController(m_selectionController);  // 输入控制器需要选择控制器
    m_inputController->setDocumentView(m_documentView);                // 输入控制器需要文档视图
    
    // 建立信号连接以实现组件间的状态同步
    // 当选择状态改变时，更新视图显示
    connect(m_selectionController, &SelectionController::selectionChanged, 
            this, &TextEditorWidget::onSelectionChanged);
    
    // 当文档视图的选择状态改变时（如鼠标点击），更新视图显示
    connect(m_documentView, &DocumentView::selectionChanged, 
            this, &TextEditorWidget::onSelectionChanged);
    
    // 当文档内容改变时，更新视图布局
    connect(m_documentController, &DocumentController::documentChanged,
            this, &TextEditorWidget::onDocumentChanged);
    
    // 注意：此时文档控制器还没有设置具体文档，
    // 需要通过setDocument()方法在外部设置实际的文档对象
}

/**
 * @brief 文档变化处理槽函数
 * 
 * 当文档内容发生任何修改时被调用，负责同步更新UI显示。
 * 这个槽函数通过信号-槽机制与文档控制器连接。
 * 
 * 主要功能：
 * - 触发文档视图重新计算布局
 * - 更新所有相关组件的显示状态
 * - 确保UI与数据模型保持一致
 */
void TextEditorWidget::onDocumentChanged()
{
    // 调用文档视图的布局更新方法
    // 这会重新计算所有段落和运行的位置、大小等布局信息
    m_documentView->updateLayout();
}

/**
 * @brief 选择状态变化处理槽函数
 * @param selection 新的选择范围对象
 * 
 * 当用户改变文本选择状态时被调用，负责更新相关的UI组件。
 * 这个槽函数通过信号-槽机制与选择控制器连接。
 * 
 * 主要功能：
 * - 将新的选择状态传递给文档视图进行可视化显示
 * - 确保当前光标位置在可视区域内
 * - 维护选择状态与视图显示的一致性
 */
void TextEditorWidget::onSelectionChanged(const Selection &selection)
{
    // 如果选择来自视图，则更新控制器
    if (m_selectionController->selection() != selection) {
        m_selectionController->setSelection(selection);
    }
    // 如果选择来自控制器，则更新视图
    if (m_documentView->selection() != selection) {
        m_documentView->setSelection(selection);
    }
    
    // 确保光标当前位置可见
    // 如果光标移出了可视区域，会自动滚动到合适位置
    m_documentView->ensureCursorVisible();
    
    // 更新状态栏
    updateStatusBar();
}

/**
 * @brief 获取文档视图组件
 * @return DocumentView* 文档视图组件的指针
 * 
 * 提供对外访问文档视图组件的接口，允许外部代码：
 * - 直接操作视图属性
 * - 获取视图状态信息
 * - 进行视图相关的特殊操作
 * 
 * 注意：返回的是原始指针，调用者需要注意对象生命周期管理。
 */
DocumentView *TextEditorWidget::documentView() const
{
    return m_documentView;
}

/**
 * @brief 获取当前编辑的文档对象
 * @return Document* 当前文档对象的指针
 * 
 * 通过文档视图间接获取正在编辑的文档实例。
 * 
 * 使用场景：
 * - 获取文档内容进行保存
 * - 查询文档状态信息
 * - 执行文档级别的操作
 * 
 * 注意：如果尚未设置文档或文档已被销毁，可能返回nullptr。
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
    
    // 更新状态栏，显示初始光标位置
    updateStatusBar();
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
 * @brief 重做操作
 * 
 * 执行重做功能，重新执行之前被撤销的操作。
 * 
 * 标准重做操作流程：
 * 1. 检查重做栈是否为空
 * 2. 从重做栈中取出操作
 * 3. 重新执行该操作
 * 4. 更新所有相关组件的状态
 * 5. 刷新UI显示
 * 
 * 重做功能通常与撤销功能配合使用。
 * 
 * TODO: 需要实现具体的重做逻辑
 */
void TextEditorWidget::redo()
{
    // TODO: 实现重做功能
    // 可能的实现步骤：
    // 1. 调用文档控制器的重做方法
    // 2. 更新视图显示
    // 3. 更新光标和选择状态
}

/**
 * @brief 更新状态栏
 * 更新光标位置信息显示
 */
void TextEditorWidget::updateStatusBar()
{
    if (m_documentView && m_documentView->cursor()) {
        Selection::Position pos = m_documentView->cursor()->position();
        int line = pos.paragraph + 1; // 行号从1开始
        int column = pos.position + 1; // 列号从1开始
        
        // 获取光标坐标（场景坐标）
        QPointF cursorPos = m_documentView->pointFromPosition(pos);
        
        // 获取光标左右字符
        QString leftChar = "无";
        QString rightChar = "无";
        qreal leftCharWidth = 0;
        qreal rightCharWidth = 0;
        QPointF leftCharPos(0, 0);
        QPointF rightCharPos(0, 0);
        
        if (m_documentView->document()) {
            Document *doc = m_documentView->document();
            if (pos.paragraph >= 0 && pos.paragraph < doc->paragraphCount()) {
                Paragraph paragraph = doc->paragraph(pos.paragraph);
                QString text = paragraph.text();
                
                // 获取字体度量 - 使用浮点版本
                QFont font("Microsoft YaHei", 12);
                QFontMetricsF metrics(font);
                qreal lineHeight = metrics.height();
                
                // 获取左侧字符（光标左侧的字符）
                if (pos.position > 0 && pos.position <= text.length()) {
                    leftChar = text.mid(pos.position - 1, 1);
                    leftCharWidth = metrics.horizontalAdvance(leftChar);
                    
                    // 计算左侧字符的位置（场景坐标）
                    Selection::Position leftPos = pos;
                    leftPos.position--;
                    leftCharPos = m_documentView->pointFromPosition(leftPos);
                }
                
                // 获取右侧字符（光标右侧的字符）
                if (pos.position >= 0 && pos.position < text.length()) {
                    rightChar = text.mid(pos.position, 1);
                    rightCharWidth = metrics.horizontalAdvance(rightChar);
                    
                    // 计算右侧字符的位置（场景坐标）
                    Selection::Position rightPos = pos;
                    rightCharPos = m_documentView->pointFromPosition(rightPos);
                }
            }
        }
        
        // 简化状态栏显示，只显示关键信息
        QString calculationInfo = QString("光标定位算法: 最近间隙");
        QString calculationInfo2 = QString("坐标计算: 累积宽度");
        QString calculationInfo3 = QString("精度: 浮点计算");
        
        // 使用3行显示状态栏信息
        QString statusText = QString("第%1行, 第%2列 | 光标位置索引:%3 | 场景坐标: (%4,%5)\n")
            .arg(line).arg(column).arg(pos.position).arg(cursorPos.x(), 0, 'f', 2).arg(cursorPos.y(), 0, 'f', 2);
        
        // 获取文本内容用于索引验证
        QString text = "";
        if (m_documentView->document() && pos.paragraph >= 0 && pos.paragraph < m_documentView->document()->paragraphCount()) {
            Paragraph paragraph = m_documentView->document()->paragraph(pos.paragraph);
            text = paragraph.text();
        }
        
        statusText += QString("左字符索引:%1 字符:'%2' (坐标:%3,%4 宽:%5) | 右字符索引:%6 字符:'%7' (坐标:%8,%9 宽:%10)\n")
            .arg(pos.position > 0 && pos.position <= text.length() ? pos.position - 1 : -1).arg(leftChar)
            .arg(leftCharPos.x(), 0, 'f', 2).arg(leftCharPos.y(), 0, 'f', 2).arg(leftCharWidth, 0, 'f', 2)
            .arg(pos.position >= 0 && pos.position < text.length() ? pos.position : -1).arg(rightChar)
            .arg(rightCharPos.x(), 0, 'f', 2).arg(rightCharPos.y(), 0, 'f', 2).arg(rightCharWidth, 0, 'f', 2);
        
        statusText += QString("%1 | %2 | %3")
            .arg(calculationInfo).arg(calculationInfo2).arg(calculationInfo3);
        
        m_cursorPositionLabel->setText(statusText);
    }
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
 * @param event 鼠标事件对象，包含点击位置、按键等信息
 * 
 * 处理鼠标点击相关的交互操作：
 * 
 * 主要功能：
 * 1. 调用父类的基础鼠标事件处理
 * 2. 确保编辑器获得键盘焦点
 * 3. 为后续的输入法操作做好准备
 * 
 * 焦点管理的重要性：
 * - 只有获得焦点的控件才能接收键盘输入
 * - 输入法需要焦点控件才能正确工作
 * - 点击编辑区域应该自动获得焦点
 * 
 * 注意：具体的文本选择逻辑通常由文档视图组件处理。
 */
void TextEditorWidget::mousePressEvent(QMouseEvent *event)
{
    // 先调用父类的鼠标事件处理
    // 这样可以确保基础的鼠标交互正常工作
    QWidget::mousePressEvent(event);
    
    // 确保当前编辑器获得焦点
    // 这对于启用键盘输入和输入法支持至关重要
    setFocus();
}

/**
 * @brief 输入法事件处理
 * @param event 输入法事件对象，包含候选词、预编辑文本等信息
 * 
 * 处理来自输入法框架的各种事件：
 * 
 * 支持的功能：
 * 1. 预编辑文本显示（输入法候选过程中的临时文本）
 * 2. 候选词列表管理
 * 3. 最终文本提交
 * 4. 输入法状态切换
 * 
 * 处理流程：
 * 1. 将事件委托给专业的输入控制器处理
 * 2. 输入控制器负责具体的文本插入和光标管理
 * 3. 调用父类方法确保兼容性
 * 
 * 这种设计使得输入法支持模块化，便于维护和扩展。
 */
void TextEditorWidget::inputMethodEvent(QInputMethodEvent *event)
{
    if (m_inputController) {
        m_inputController->handleInputMethodEvent(event);
        if (event->isAccepted()) {
            return; // 已处理，不再调用父类
        }
    }
    QWidget::inputMethodEvent(event);
}

/**
 * @brief 输入法查询处理
 * @param query 查询类型，指定需要的信息类型
 * @return QVariant 查询结果，根据查询类型返回相应的数据
 * 
 * 响应输入法框架的查询请求，提供必要的编辑器状态信息：
 * 
 * 常见查询类型包括：
 * - Qt::ImCursorRectangle: 光标位置和大小
 * - Qt::ImFont: 当前字体信息
 * - Qt::ImAnchorPosition: 选择锚点位置
 * - Qt::ImCursorPosition: 当前光标位置
 * - Qt::ImSurroundingText: 周围文本内容
 * 
 * 实现方式：
 * 1. 将查询请求委托给文档视图处理
 * 2. 文档视图了解具体的文本布局和光标信息
 * 3. 如果没有文档视图，则使用父类的默认实现
 * 
 * 这种委托模式保证了输入法能够获得准确的位置和状态信息。
 */
QVariant TextEditorWidget::inputMethodQuery(Qt::InputMethodQuery query) const
{
    // 委托给文档视图处理具体的输入法查询
    // 文档视图掌握着光标位置、文本布局等关键信息
    if (m_documentView) {
        return m_documentView->inputMethodQueryPublic(query);
    }
    
    // 如果没有文档视图，使用QWidget的默认实现
    return QWidget::inputMethodQuery(query);
}
