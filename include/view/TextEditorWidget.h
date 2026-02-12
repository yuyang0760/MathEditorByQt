#ifndef TEXTEDITORWIDGET_H
#define TEXTEDITORWIDGET_H

#include "view/DocumentView.h"
#include <QWidget>
#include <QVBoxLayout>

class TextEditorWidget : public QWidget
{
    Q_OBJECT
public:
    TextEditorWidget(QWidget *parent = nullptr);
    
    // 获取文档视图
    DocumentView *documentView() const;
    
    // 获取和设置文档
    Document *document() const;
    void setDocument(Document *document);
    
    // 编辑操作
    void cut();
    void copy();
    void paste();
    void undo();
    void redo();
    
protected:
    // 重写的事件处理方法
    void keyPressEvent(QKeyEvent *event) override;
    
private:
    QVBoxLayout *m_layout;
    DocumentView *m_documentView;
};

#endif // TEXTEDITORWIDGET_H
