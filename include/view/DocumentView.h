// ============================================================================
// DocumentView.h
// 文档视图头文件
// 提供文档的图形化显示和交互功能
// ============================================================================

#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMap>
#include "core/Document.h"
#include "core/Selection.h"
#include "view/Cursor.h"
#include "view/ParagraphLayout.h"

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
    void resizeEvent(QResizeEvent *event) override;

public:
    // 坐标转换
    Position positionFromPoint(const QPointF &point) const;
    QPointF pointFromPosition(const Position &pos) const;

private:
    void updateInputMethod();
    void clearGraphicsItems();
    void rebuildScene();
    
    /**
     * @brief 获取或创建段落布局
     */
    ParagraphLayout *getOrCreateLayout(int paragraphIndex);
    
    /**
     * @brief 更新所有段落布局
     */
    void updateAllLayouts();
    
    /**
     * @brief 清除所有布局缓存
     */
    void clearLayouts();

    QGraphicsScene *m_scene;
    Document *m_document;
    Selection m_selection;
    Cursor *m_cursor;
    bool m_selecting;
    Position m_selectionStart;
    
    QMap<int, ParagraphLayout*> m_paragraphLayouts;  ///< 段落布局缓存
    qreal m_maxWidth;  ///< 最大布局宽度
};

#endif // DOCUMENTVIEW_H