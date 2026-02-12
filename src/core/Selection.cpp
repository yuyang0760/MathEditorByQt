// ============================================================================
// Selection.cpp
// 选择类的实现文件
// 表示文档中的文本选择范围
// ============================================================================

#include "core/Selection.h"

/**
 * @brief 构造函数
 * 创建一个空选择
 */
Selection::Selection()
{
}

/**
 * @brief 构造函数
 * @param start 起始位置
 * @param end 结束位置
 */
Selection::Selection(const Position &start, const Position &end)
    : m_start(start), m_end(end)
{
}

/**
 * @brief 获取起始位置
 * @return 起始位置
 */
Selection::Position Selection::start() const
{
    return m_start;
}

/**
 * @brief 设置起始位置
 * @param position 起始位置
 */
void Selection::setStart(const Position &position)
{
    m_start = position;
}

/**
 * @brief 获取结束位置
 * @return 结束位置
 */
Selection::Position Selection::end() const
{
    return m_end;
}

/**
 * @brief 设置结束位置
 * @param position 结束位置
 */
void Selection::setEnd(const Position &position)
{
    m_end = position;
}

/**
 * @brief 获取归一化的起始位置（确保start <= end）
 * @return 归一化的起始位置
 */
Selection::Position Selection::normalizedStart() const
{
    if (m_start < m_end)
        return m_start;
    return m_end;
}

/**
 * @brief 获取归一化的结束位置（确保start <= end）
 * @return 归一化的结束位置
 */
Selection::Position Selection::normalizedEnd() const
{
    if (m_start < m_end)
        return m_end;
    return m_start;
}

/**
 * @brief 检查是否为空选择
 * @return 是否为空选择
 */
bool Selection::isEmpty() const
{
    return m_start == m_end;
}

/**
 * @brief 扩展选择到指定位置
 * @param position 扩展到的位置
 */
void Selection::extend(const Position &position)
{
    m_end = position;
}

/**
 * @brief 收缩选择到起始位置
 */
void Selection::collapseToStart()
{
    m_end = m_start;
}

/**
 * @brief 收缩选择到结束位置
 */
void Selection::collapseToEnd()
{
    m_start = m_end;
}

/**
 * @brief 比较两个选择是否相等
 * @param other 另一个选择
 * @return 是否相等
 */
bool Selection::operator==(const Selection &other) const
{
    return m_start == other.m_start && m_end == other.m_end;
}

/**
 * @brief 比较两个选择是否不相等
 * @param other 另一个选择
 * @return 是否不相等
 */
bool Selection::operator!=(const Selection &other) const
{
    return !(*this == other);
}
