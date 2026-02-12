// ============================================================================
// Cursor.h
// 光标类的头文件
// 表示文档中的光标，负责显示和管理光标状态
// ============================================================================

#ifndef CURSOR_H
#define CURSOR_H

#include "core/Selection.h"
#include <QGraphicsItem>
#include <QTimer>
#include <QObject>
#include <QRectF>
#include <QPainter>

/**
 * @class Cursor
 * @brief 光标类
 * 
 * 表示文档中的光标，负责显示和管理光标状态。
 * 继承自QObject和QGraphicsItem，支持闪烁功能。
 */
class Cursor : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     * @param graphicsParent 图形父对象
     */
    Cursor(QObject *parent = nullptr, QGraphicsItem *graphicsParent = nullptr);
    
    /**
     * @brief 设置光标位置
     * @param position 光标位置
     */
    void setPosition(const Selection::Position &position);
    
    /**
     * @brief 获取光标位置
     * @return 光标位置
     */
    Selection::Position position() const;
    
    /**
     * @brief 显示光标
     */
    void show();
    
    /**
     * @brief 隐藏光标
     */
    void hide();
    
    /**
     * @brief 检查光标是否可见
     * @return 光标是否可见
     */
    bool isVisible() const;
    
    /**
     * @brief 开始闪烁
     */
    void startBlinking();
    
    /**
     * @brief 停止闪烁
     */
    void stopBlinking();
    
    /**
     * @brief 获取光标边界
     * @return 光标边界
     */
    QRectF boundingRect() const override;
    
    /**
     * @brief 绘制光标
     * @param painter 画笔
     * @param option 样式选项
     * @param widget 部件
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    
private slots:
    /**
     * @brief 切换光标可见性（用于闪烁）
     */
    void toggleVisibility();
    
private:
    /**
     * @brief 光标位置
     */
    Selection::Position m_position;
    
    /**
     * @brief 光标是否可见
     */
    bool m_visible;
    
    /**
     * @brief 闪烁定时器
     */
    QTimer *m_blinkTimer;
    
    /**
     * @brief 闪烁间隔（毫秒）
     */
    static const int BLINK_INTERVAL = 500; // 闪烁间隔（毫秒）
};

#endif // CURSOR_H
