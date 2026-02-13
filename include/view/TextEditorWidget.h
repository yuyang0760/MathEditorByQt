#ifndef TEXTEDITORWIDGET_H
#define TEXTEDITORWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QStatusBar>
#include "view/DocumentView.h"
#include "controller/DocumentController.h"
#include "controller/SelectionController.h"
#include "controller/InputController.h"

class TextEditorWidget : public QWidget {
    Q_OBJECT
public:
    TextEditorWidget(QWidget *parent = nullptr);

    void setDocument(Document *document);
    Document *document() const;

protected:
    void keyPressEvent(QKeyEvent *event) override;
    void inputMethodEvent(QInputMethodEvent *event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

private slots:
    void onSelectionChanged(const Selection &selection);
    void onMousePositionChanged(const QPointF &scenePos, const QPoint &viewPos);

private:
    void setupUI();
    void setupConnections();
    void updateStatusBar();

    QVBoxLayout *m_layout;
    DocumentView *m_documentView;
    QStatusBar *m_statusBar;
    
    DocumentController *m_documentController;
    SelectionController *m_selectionController;
    InputController *m_inputController;
    
    Document *m_document;
};

#endif // TEXTEDITORWIDGET_H