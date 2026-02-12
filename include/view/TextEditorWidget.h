// ============================================================================
// TextEditorWidget.h
// 文本编辑器部件类的头文件
// 主编辑器部件，整合了文档视图和各种控制器
// ============================================================================

#ifndef TEXTEDITORWIDGET_H
#define TEXTEDITORWIDGET_H

#include "view/DocumentView.h"
#include "controller/InputController.h"
#include "controller/DocumentController.h"
#include "controller/SelectionController.h"
#include "core/Selection.h"
#include <QWidget>
#include <QVBoxLayout>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QInputMethodEvent>
#include <QVariant>

class Document;

/**
 * @class TextEditorWidget
 * @brief 文本编辑器部件类
 * 
 * 主编辑器部件，整合了文档视图和各种控制器。
 * 负责处理用户输入，协调各个控制器的工作，提供编辑操作的接口。
 */
class TextEditorWidget : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父部件
     */
    TextEditorWidget(QWidget *parent = nullptr);
    
    /**
     * @brief 获取文档视图
     * @return 文档视图
     */
    DocumentView *documentView() const;
    
    /**
     * @brief 获取文档
     * @return 当前文档
     */
    Document *document() const;
    
    /**
     * @brief 设置文档
     * @param document 要设置的文档
     */
    void setDocument(Document *document);
    
    /**
     * @brief 剪切操作
     * 剪切选中的文本到剪贴板
     */
    void cut();
    
    /**
     * @brief 复制操作
     * 复制选中的文本到剪贴板
     */
    void copy();
    
    /**
     * @brief 粘贴操作
     * 从剪贴板粘贴文本到当前位置
     */
    void paste();
    
    /**
     * @brief 撤销操作
     * 撤销上一步操作
     */
    void undo();
    
    /**
     * @brief 重做操作
     * 重做上一步撤销的操作
     */
    void redo();
    
protected:
    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件
     */
    void keyPressEvent(QKeyEvent *event) override;
    
    /**
     * @brief 输入法事件处理
     * @param event 输入法事件
     */
    void inputMethodEvent(QInputMethodEvent *event) override;
    
    /**
     * @brief 输入法查询
     * @param query 查询类型
     * @return 查询结果
     */
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;
    
private slots:
    /**
     * @brief 选择变化槽
     * @param selection 新的选择
     */
    void onSelectionChanged(const Selection &selection);
    
    /**
     * @brief 文档变化槽
     * 当文档发生变化时调用
     */
    void onDocumentChanged();
    
private:
    /**
     * @brief 布局
     */
    QVBoxLayout *m_layout;
    
    /**
     * @brief 文档视图
     */
    DocumentView *m_documentView;
    
    /**
     * @brief 输入控制器
     */
    InputController *m_inputController;
    
    /**
     * @brief 文档控制器
     */
    DocumentController *m_documentController;
    
    /**
     * @brief 选择控制器
     */
    SelectionController *m_selectionController;
};

#endif // TEXTEDITORWIDGET_H
