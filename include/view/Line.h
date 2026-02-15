// ============================================================================
// Line.h
// 行类头文件
// 表示文档中的一行文本行，包含该行的布局信息
// ============================================================================

#ifndef LINE_H
#define LINE_H

#include <QString>
#include <QRectF>
#include <QList>
#include "core/Selection.h"

/**
 * @brief 行片段类
 * 
 * 表示一行中的一个片段（一个 TextRun 或 MathObject 的一部分）
 */
struct LineSegment {
    int itemIndex;      ///< 对应的段落项索引
    int offsetStart;    ///< 在项中的起始偏移
    int offsetEnd;      ///< 在项中的结束偏移
    qreal x;           ///< 水平位置
    qreal width;       ///< 宽度
    qreal ascent;       ///< 上行高度（从基线到顶部）
    qreal descent;      ///< 下行高度（从基线到底部）

    LineSegment() 
        : itemIndex(0), offsetStart(0), offsetEnd(0), 
          x(0), width(0), ascent(0), descent(0) {}

    LineSegment(int itemIdx, int offStart, int offEnd, 
                qreal xPos, qreal w, qreal a, qreal d)
        : itemIndex(itemIdx), offsetStart(offStart), offsetEnd(offEnd),
          x(xPos), width(w), ascent(a), descent(d) {}
};

/**
 * @brief 行类
 * 
 * 表示文档中的一行文本，包含该行的布局信息
 */
class Line {
public:
    Line();
    
    /**
     * @brief 构造函数
     * @param paragraphIndex 段落索引
     * @param lineIndex 行在段落中的索引
     */
    Line(int paragraphIndex, int lineIndex);
    
    /**
     * @brief 获取段落索引
     */
    int paragraphIndex() const;
    
    /**
     * @brief 获取行在段落中的索引
     */
    int lineIndex() const;
    
    /**
     * @brief 设置行位置和大小
     */
    void setRect(const QRectF &rect);
    
    /**
     * @brief 获取行的矩形区域
     */
    QRectF rect() const;
    
    /**
     * @brief 设置行的基线位置（相对于行顶部）
     */
    void setBaseline(qreal baseline);
    
    /**
     * @brief 获取行的基线位置（相对于行顶部）
     */
    qreal baseline() const;
    
    /**
     * @brief 添加行片段
     */
    void addSegment(const LineSegment &segment);
    
    /**
     * @brief 获取所有行片段
     */
    const QList<LineSegment> &segments() const;
    
    /**
     * @brief 获取行起始位置
     */
    Position startPosition() const;
    
    /**
     * @brief 设置行起始位置
     */
    void setStartPosition(const Position &pos);
    
    /**
     * @brief 获取行结束位置
     */
    Position endPosition() const;
    
    /**
     * @brief 设置行结束位置
     */
    void setEndPosition(const Position &pos);
    
    /**
     * @brief 检查位置是否在该行内
     */
    bool containsPosition(const Position &pos) const;
    
    /**
     * @brief 根据水平位置查找对应的片段和偏移
     * @param x 水平坐标
     * @param[out] segmentIndex 返回的片段索引
     * @param[out] offset 返回的偏移
     */
    bool findPositionAtX(qreal x, int &segmentIndex, int &offset) const;
    
    /**
     * @brief 清空行
     */
    void clear();

private:
    int m_paragraphIndex;    ///< 段落索引
    int m_lineIndex;         ///< 行在段落中的索引
    QRectF m_rect;          ///< 行的矩形区域
    qreal m_baseline;       ///< 行的基线位置（相对于行顶部）
    QList<LineSegment> m_segments; ///< 行片段列表
    Position m_startPosition;  ///< 行起始位置
    Position m_endPosition;    ///< 行结束位置
};

#endif // LINE_H
