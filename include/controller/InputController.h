// ============================================================================
// InputController.h
// 输入控制器类的头文件
// 负责处理用户输入事件，如键盘输入、鼠标事件和输入法事件
// ============================================================================

#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "DocumentController.h"
#include "SelectionController.h"
#include <QObject>
#include <QInputMethodEvent>
#include <QKeyEvent>
#include <QPoint>

class DocumentView;

/**
 * @class InputController
 * @brief 输入控制器类
 * 
 * 负责处理用户输入事件，如键盘输入、鼠标事件和输入法事件。
 * 将用户输入转换为对文档的操作，是用户与文档之间的桥梁。
 */
class InputController : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    InputController(QObject *parent = nullptr);
    
    /**
     * @brief 设置文档控制器
     * @param controller 文档控制器
     */
    void setDocumentController(DocumentController *controller);
    
    /**
     * @brief 设置选择控制器
     * @param controller 选择控制器
     */
    void setSelectionController(SelectionController *controller);
    
    /**
     * @brief 设置文档视图
     * @param view 文档视图
     */
    void setDocumentView(DocumentView *view);
    
    /**
     * @brief 处理键盘按下事件
     * @param event 键盘事件
     */
    void handleKeyPress(QKeyEvent *event);
    
    /**
     * @brief 处理输入法事件
     * @param event 输入法事件
     */
    void handleInputMethodEvent(QInputMethodEvent *event);
    
    /**
     * @brief 处理鼠标按下事件
     * @param point 鼠标位置
     */
    void handleMousePress(const QPoint &point);
    
    /**
     * @brief 处理鼠标移动事件
     * @param point 鼠标位置
     */
    void handleMouseMove(const QPoint &point);
    
    /**
     * @brief 处理鼠标释放事件
     * @param point 鼠标位置
     */
    void handleMouseRelease(const QPoint &point);
    
    /**
     * @brief 更新输入法状态
     */
    void updateInputMethod();
    
private:
    /**
     * @brief 文档控制器
     */
    DocumentController *m_documentController;
    
    /**
     * @brief 选择控制器
     */
    SelectionController *m_selectionController;
    
    /**
     * @brief 文档视图
     */
    DocumentView *m_documentView;
    
    /**
     * @brief 组合文本（输入法输入过程中的临时文本）
     */
    QString m_composingText;
};

#endif // INPUTCONTROLLER_H
