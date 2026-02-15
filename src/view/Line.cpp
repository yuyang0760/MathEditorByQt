// ============================================================================
// Line.cpp
// 行类实现文件
// ============================================================================

#include "view/Line.h"

/**
 * @brief 构造函数
 */
Line::Line()
    : m_paragraphIndex(0), m_lineIndex(0), m_baseline(0) {}

/**
 * @brief 构造函数
 * @param paragraphIndex 段落索引
 * @param lineIndex 行在段落中的索引
 */
Line::Line(int paragraphIndex, int lineIndex)
    : m_paragraphIndex(paragraphIndex), m_lineIndex(lineIndex), m_baseline(0) {}

/**
 * @brief 获取段落索引
 */
int Line::paragraphIndex() const {
    return m_paragraphIndex;
}

/**
 * @brief 获取行在段落中的索引
 */
int Line::lineIndex() const {
    return m_lineIndex;
}

/**
 * @brief 设置行位置和大小
 */
void Line::setRect(const QRectF &rect) {
    m_rect = rect;
}

/**
 * @brief 获取行的矩形区域
 */
QRectF Line::rect() const {
    return m_rect;
}

/**
 * @brief 设置行的基线位置（相对于行顶部）
 */
void Line::setBaseline(qreal baseline) {
    m_baseline = baseline;
}

/**
 * @brief 获取行的基线位置（相对于行顶部）
 */
qreal Line::baseline() const {
    return m_baseline;
}

/**
 * @brief 添加行片段
 */
void Line::addSegment(const LineSegment &segment) {
    m_segments.append(segment);
}

/**
 * @brief 获取所有行片段
 */
const QList<LineSegment> &Line::segments() const {
    return m_segments;
}

/**
 * @brief 获取行起始位置
 */
Position Line::startPosition() const {
    return m_startPosition;
}

/**
 * @brief 设置行起始位置
 */
void Line::setStartPosition(const Position &pos) {
    m_startPosition = pos;
}

/**
 * @brief 获取行结束位置
 */
Position Line::endPosition() const {
    return m_endPosition;
}

/**
 * @brief 设置行结束位置
 */
void Line::setEndPosition(const Position &pos) {
    m_endPosition = pos;
}

/**
 * @brief 检查位置是否在该行内
 */
bool Line::containsPosition(const Position &pos) const {
    if (pos.paragraphIndex != m_paragraphIndex) {
        return false;
    }
    
    // 比较起始和结束位置
    if (pos < m_startPosition) {
        return false;
    }
    if (pos > m_endPosition) {
        return false;
    }
    return true;
}

/**
 * @brief 根据水平位置查找对应的片段和偏移
 */
bool Line::findPositionAtX(qreal x, int &segmentIndex, int &offset) const {
    if (m_segments.isEmpty()) {
        return false;
    }
    
    // 找到包含 x 的片段
    for (int i = 0; i < m_segments.size(); ++i) {
        const LineSegment &seg = m_segments[i];
        if (x >= seg.x && x < seg.x + seg.width) {
            segmentIndex = i;
            // 简单处理：根据 x 位置估算偏移
            qreal ratio = (x - seg.x) / seg.width;
            offset = qBound(seg.offsetStart, 
                           seg.offsetStart + static_cast<int>(ratio * (seg.offsetEnd - seg.offsetStart)),
                           seg.offsetEnd);
            return true;
        }
    }
    
    // 如果 x 在所有片段的左边
    if (x < m_segments.first().x) {
        segmentIndex = 0;
        offset = m_segments.first().offsetStart;
        return true;
    }
    
    // 如果 x 在所有片段的右边
    segmentIndex = m_segments.size() - 1;
    offset = m_segments.last().offsetEnd;
    return true;
}

/**
 * @brief 清空行
 */
void Line::clear() {
    m_rect = QRectF();
    m_baseline = 0;
    m_segments.clear();
    m_startPosition = Position();
    m_endPosition = Position();
}
