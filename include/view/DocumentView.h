#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include "core/Document.h"
#include "core/Selection.h"
#include "view/Cursor.h"
#include <QGraphicsView>
#include <QGraphicsScene>

class DocumentView : public QGraphicsView
{
    Q_OBJECT
public:
    DocumentView(QWidget *parent = nullptr);
    
    // 设置文档
    void setDocument(Document *document);
    Document *document() const;
    
    // 设置选择
    void setSelection(const Selection &selection);
    Selection selection() const;
    
    // 获取光标
    Cursor *cursor() const;
    
    // 视图操作
    void updateLayout();
    void ensureCursorVisible();
    
protected:
    // 重写的事件处理方法
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    
private:
    // 辅助方法
    Selection::Position positionFromPoint(const QPoint &point) const;
    QPoint pointFromPosition(const Selection::Position &position) const;
    
private:
    QGraphicsScene *m_scene;
    Document *m_document;
    Selection m_selection;
    Cursor *m_cursor;
    bool m_selecting;
    Selection::Position m_selectionStart;
};

#endif // DOCUMENTVIEW_H
