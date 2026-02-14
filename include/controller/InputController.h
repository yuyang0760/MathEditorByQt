// ============================================================================
// InputController.h
// 输入控制器头文件
// 处理键盘输入和输入法事件，转换为文档编辑操作
// ============================================================================

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <QObject>
#include <QKeyEvent>
#include <QInputMethodEvent>

class DocumentController;
class SelectionController;
class DocumentView;

/**
 * @brief 输入控制器类
 * 
 * InputController类负责处理用户输入，包括键盘事件和输入法事件。
 * 它将输入事件转换为相应的文档编辑操作，如插入文本、删除文本等。
 * 
 * 主要功能：
 * - 键盘事件处理（字母、数字、特殊键）
 * - 输入法事件处理（中文、日文等复杂输入）
 * - 组合文本管理
 * - 与其他控制器和视图的协作
 */
class InputController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    InputController(QObject *parent = nullptr);

    /**
     * @brief 设置文档控制器
     * @param controller 文档控制器指针
     */
    void setDocumentController(DocumentController *controller);
    
    /**
     * @brief 设置选择控制器
     * @param controller 选择控制器指针
     */
    void setSelectionController(SelectionController *controller);
    
    /**
     * @brief 设置文档视图
     * @param view 文档视图指针
     */
    void setDocumentView(DocumentView *view);

    /**
     * @brief 处理键盘按下事件
     * @param event 键盘事件对象
     */
    void handleKeyPress(QKeyEvent *event);
    
    /**
     * @brief 处理输入法事件
     * @param event 输入法事件对象
     */
    void handleInputMethodEvent(QInputMethodEvent *event);
    
    /**
     * @brief 获取当前组合文本
     * @return 组合文本字符串
     */
    QString composingText() const { return m_composingText; }

private:
    DocumentController *m_documentController; ///< 文档控制器指针
    SelectionController *m_selectionController; ///< 选择控制器指针
    DocumentView *m_documentView; ///< 文档视图指针
    QString m_composingText; ///< 当前组合文本
};

#endif // INPUTCONTROLLER_H