#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include "DocumentController.h"
#include "SelectionController.h"
#include <QObject>
#include <QInputMethodEvent>

class InputController : public QObject
{
    Q_OBJECT
public:
    InputController(QObject *parent = nullptr);
    
    // 设置控制器
    void setDocumentController(DocumentController *controller);
    void setSelectionController(SelectionController *controller);
    
    // 处理输入事件
    void handleKeyPress(QKeyEvent *event);
    void handleInputMethodEvent(QInputMethodEvent *event);
    void handleMousePress(const QPoint &point);
    void handleMouseMove(const QPoint &point);
    void handleMouseRelease(const QPoint &point);
    
    // 输入法相关
    void updateInputMethod();
    
private:
    DocumentController *m_documentController;
    SelectionController *m_selectionController;
    QString m_composingText;
};

#endif // INPUTCONTROLLER_H
