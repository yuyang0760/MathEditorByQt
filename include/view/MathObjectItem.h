// ============================================================================
// MathObjectItem.h
// 数学对象图形项头文件
// 负责数学对象的图形化显示
// ============================================================================

#ifndef MATHOBJECTITEM_H
#define MATHOBJECTITEM_H

#include <QGraphicsItem>
#include "core/MathObject.h"

/**
 * @brief 数学对象图形项类
 * 
 * MathObjectItem类负责数学对象的图形化显示，继承自QGraphicsItem。
 * 它将数学对象渲染为图形项，在文档视图中显示。
 * 
 * 主要功能：
 * - 数学对象的图形化显示
 * - 边界矩形计算
 * - 数学对象数据管理
 */
class MathObjectItem : public QGraphicsItem {
public:
    /**
     * @brief 构造函数
     * @param obj 数学对象
     * @param parent 父图形项
     */
    MathObjectItem(const MathObject &obj, QGraphicsItem *parent = nullptr);

    /**
     * @brief 获取边界矩形
     * @return 图形项的边界矩形
     */
    QRectF boundingRect() const override;
    
    /**
     * @brief 绘制图形项
     * @param painter 绘图设备
     * @param option 样式选项
     * @param widget 绘制部件
     */
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;

    /**
     * @brief 设置数学对象
     * @param obj 新的数学对象
     */
    void setObject(const MathObject &obj);
    
    /**
     * @brief 获取当前数学对象
     * @return 当前数学对象
     */
    MathObject object() const;

    static constexpr qreal WIDTH = 30;  ///< 数学对象默认宽度
    static constexpr qreal HEIGHT = 20; ///< 数学对象默认高度

private:
    MathObject m_obj; ///< 内部存储的数学对象
};

#endif // MATHOBJECTITEM_H