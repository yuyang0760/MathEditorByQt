// ============================================================================
// Cursor.h
// 光标类头文件
// 定义文档中的光标显示和闪烁效果
// ============================================================================

#ifndef CURSOR_H
#define CURSOR_H

#include "core/Selection.h"
#include <QGraphicsItem>
#include <QTimer>
#include <QObject>

/**
 * @brief 光标类
 * 
 * Cursor类表示文档中的插入光标，继承自QObject和QGraphicsItem。
 * 提供光标显示、闪烁效果和位置管理功能。
 * 
 * 主要功能：
 * - 光标位置管理
 * - 闪烁动画效果
 * - 图形显示和绘制
 * - 可见性控制
 */
class Cursor : public QObject, public QGraphicsItem {
    Q_OBJECT
    Q_INTERFACES(QGraphicsItem)
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     * @param graphicsParent 图形父项指针
     */
    Cursor(QObject *parent = nullptr, QGraphicsItem *graphicsParent = nullptr);

    /**
     * @brief 设置光标位置
     * @param position 新的位置
     */
    void setPosition(const Position &position);
    
    /**
     * @brief 获取当前光标位置
     * @return 当前光标位置
     */
    Position position() const;

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
     * @return 如果光标可见返回true
     */
    bool isVisible() const;

    /**
     * @brief 开始光标闪烁
     */
    void startBlinking();
    
    /**
     * @brief 停止光标闪烁
     */
    void stopBlinking();

    /**
     * @brief 获取边界矩形
     * @return 光标的边界矩形
     */
    QRectF boundingRect() const override;
    
    /**
     * @brief 绘制光标
     * @param painter 绘图设备
     * @param option 样式选项
     * @param widget 绘制部件
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

private slots:
    /**
     * @brief 切换可见性
     * 用于实现光标闪烁效果
     */
    void toggleVisibility();

private:
    Position m_position;        ///< 光标位置
    bool m_visible;            ///< 可见性标志
    QTimer *m_blinkTimer;      ///< 闪烁定时器
    static const int BLINK_INTERVAL = 500;  ///< 闪烁间隔（毫秒）
};

#endif // CURSOR_H