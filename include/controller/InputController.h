#ifndef INPUTCONTROLLER_H
#define INPUTCONTROLLER_H

#include <QObject>
#include <QKeyEvent>
#include <QInputMethodEvent>

class DocumentController;
class SelectionController;
class DocumentView;

class InputController : public QObject {
    Q_OBJECT
public:
    InputController(QObject *parent = nullptr);

    void setDocumentController(DocumentController *controller);
    void setSelectionController(SelectionController *controller);
    void setDocumentView(DocumentView *view);

    void handleKeyPress(QKeyEvent *event);
    void handleInputMethodEvent(QInputMethodEvent *event);

private:
    DocumentController *m_documentController;
    SelectionController *m_selectionController;
    DocumentView *m_documentView;
    QString m_composingText;
};

#endif // INPUTCONTROLLER_H