// ============================================================================
// DocumentView.h
// 文档视图类的头文件
// 负责显示文档内容，处理用户交互，管理光标和选择
// ============================================================================

#ifndef DOCUMENTVIEW_H
#define DOCUMENTVIEW_H

#include "core/Document.h"
#include "core/Selection.h"
#include "view/Cursor.h"
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPoint>
#include <QVariant>
#include <QGraphicsTextItem>

/**
 * @class DocumentView
 * @brief 文档视图类
 * 
 * 负责显示文档内容，处理用户交互，管理光标和选择。
 * 继承自QGraphicsView，使用QGraphicsScene来绘制文档内容。
 */
class DocumentView : public QGraphicsView
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父部件
     */
    DocumentView(QWidget *parent = nullptr);
    
    /**
     * @brief 设置文档
     * @param document 要显示的文档
     */
    void setDocument(Document *document);
    
    /**
     * @brief 获取文档
     * @return 当前文档
     */
    Document *document() const;
    
    /**
     * @brief 设置选择
     * @param selection 要设置的选择
     */
    void setSelection(const Selection &selection);
    
    /**
     * @brief 获取选择
     * @return 当前选择
     */
    Selection selection() const;
    
    /**
     * @brief 获取光标
     * @return 光标对象
     */
    Cursor *cursor() const;
    
    /**
     * @brief 更新布局
     * 重新计算文档的布局
     */
    void updateLayout();
    
    /**
     * @brief 确保光标可见
     * 调整视图以确保光标在可见区域内
     */
    void ensureCursorVisible();
    
    /**
     * @brief 输入法查询代理（公共访问）
     * @param query 查询类型
     * @return 查询结果
     */
    QVariant inputMethodQueryPublic(Qt::InputMethodQuery query) const;
    
    /**
     * @brief 显示组合文本
     * @param text 组合文本
     */
    void showComposingText(const QString &text);
    
    /**
     * @brief 隐藏组合文本
     */
    void hideComposingText();
    
protected:
    /**
     * @brief 鼠标按下事件处理
     * @param event 鼠标事件
     */
    void mousePressEvent(QMouseEvent *event) override;
    
    /**
     * @brief 鼠标移动事件处理
     * @param event 鼠标事件
     */
    void mouseMoveEvent(QMouseEvent *event) override;
    
    /**
     * @brief 鼠标释放事件处理
     * @param event 鼠标事件
     */
    void mouseReleaseEvent(QMouseEvent *event) override;
    
    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件
     */
    void keyPressEvent(QKeyEvent *event) override;
    
    /**
     * @brief 输入法查询
     * @param query 查询类型
     * @return 查询结果
     */
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;
    
private:
    /**
     * @brief 从点获取位置
     * @param point 点坐标
     * @return 文档位置
     */
    Selection::Position positionFromPoint(const QPoint &point) const;
    
    /**
     * @brief 从位置获取点
     * @param position 文档位置
     * @return 点坐标
     */
    QPoint pointFromPosition(const Selection::Position &position) const;
    
    /**
     * @brief 更新输入法
     */
    void updateInputMethod();
    
    /**
     * @brief 绘制组合文本
     * @param text 组合文本
     * @param position 位置
     */
    void drawComposingText(const QString &text, const Selection::Position &position);
    
    /**
     * @brief 清除组合文本
     */
    void clearComposingText();
    
private:
    /**
     * @brief 图形场景
     */
    QGraphicsScene *m_scene;
    
    /**
     * @brief 当前文档
     */
    Document *m_document;
    
    /**
     * @brief 当前选择
     */
    Selection m_selection;
    
    /**
     * @brief 光标
     */
    Cursor *m_cursor;
    
    /**
     * @brief 是否正在选择
     */
    bool m_selecting;
    
    /**
     * @brief 选择起始位置
     */
    Selection::Position m_selectionStart;
    
    /**
     * @brief 组合文本项
     */
    QGraphicsTextItem *m_composingTextItem;
    
    /**
     * @brief 当前组合文本
     */
    QString m_currentComposingText;
};

#endif // DOCUMENTVIEW_H
