#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include "core/Document.h"
#include "core/Selection.h"
#include "view/Cursor.h"

class DocumentView : public QGraphicsView {
    Q_OBJECT
public:
    DocumentView(QWidget *parent = nullptr);

    void setDocument(Document *document);
    Document *document() const;

    void setSelection(const Selection &selection);
    Selection selection() const;

    Cursor *cursor() const;

    void updateLayout();
    void ensureCursorVisible();

    // 用于输入法查询
    QVariant inputMethodQueryPublic(Qt::InputMethodQuery query) const;

signals:
    void selectionChanged(const Selection &selection);
    void mousePositionChanged(const QPointF &scenePos, const QPoint &viewPos);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

public:
    // 坐标转换
    Position positionFromPoint(const QPointF &point) const;
    QPointF pointFromPosition(const Position &pos) const;

private:
    void updateInputMethod();
    void clearGraphicsItems();
    void rebuildScene();

    QGraphicsScene *m_scene;
    Document *m_document;
    Selection m_selection;
    Cursor *m_cursor;
    bool m_selecting;
    Position m_selectionStart;
};

#endif // DOCUMENTVIEW_H