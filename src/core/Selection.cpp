// ============================================================================
// Selection.cpp
// 选择和位置类实现文件
// 实现文档中的位置和选择区域的基本功能
// ============================================================================

#include "core/Selection.h"

/**
 * @brief 默认构造函数
 * 创建空的选择区域，起始位置和结束位置都为(0,0,0)
 */
Selection::Selection() : m_start{0,0,0}, m_end{0,0,0} {}

/**
 * @brief 带参数构造函数
 * @param start 选择起始位置
 * @param end 选择结束位置
 */
Selection::Selection(const Position &start, const Position &end) : m_start(start), m_end(end) {}

/**
 * @brief 获取起始位置
 * @return 选择起始位置
 */
Position Selection::start() const { return m_start; }

/**
 * @brief 设置起始位置
 * @param pos 新的起始位置
 */
void Selection::setStart(const Position &pos) { m_start = pos; }

/**
 * @brief 获取结束位置
 * @return 选择结束位置
 */
Position Selection::end() const { return m_end; }

/**
 * @brief 设置结束位置
 * @param pos 新的结束位置
 */
void Selection::setEnd(const Position &pos) { m_end = pos; }

/**
 * @brief 获取规范化起始位置
 * 
 * 该方法返回规范化后的起始位置，确保起始位置小于或等于结束位置
 * 
 * @return 规范化后的起始位置
 */
Position Selection::normalizedStart() const { return m_start < m_end ? m_start : m_end; }

/**
 * @brief 获取规范化结束位置
 * 
 * 该方法返回规范化后的结束位置，确保结束位置大于或等于起始位置
 * 
 * @return 规范化后的结束位置
 */
Position Selection::normalizedEnd() const { return m_start < m_end ? m_end : m_start; }

/**
 * @brief 检查选择是否为空
 * 
 * 当起始位置和结束位置相同时，选择为空
 * 
 * @return 如果选择为空返回true
 */
bool Selection::isEmpty() const { return m_start == m_end; }

/**
 * @brief 扩展选择到指定位置
 * 
 * 该方法将选择的结束位置设置为指定位置
 * 
 * @param pos 要扩展到的位置
 */
void Selection::extend(const Position &pos) { m_end = pos; }

/**
 * @brief 折叠选择到起始位置
 * 
 * 该方法将结束位置设置为起始位置，使选择为空
 * 
 * @return 无
 */
void Selection::collapseToStart() { m_end = m_start; }

/**
 * @brief 折叠选择到结束位置
 * 
 * 该方法将起始位置设置为结束位置，使选择为空
 * 
 * @return 无
 */
void Selection::collapseToEnd() { m_start = m_end; }

/**
 * @brief 等于操作符重载
 * 
 * 当两个选择的起始位置和结束位置都相同时，认为它们相等
 * 
 * @param other 要比较的另一个选择对象
 * @return 如果两个选择相同返回true
 */
bool Selection::operator==(const Selection &other) const { return m_start == other.m_start && m_end == other.m_end; }

/**
 * @brief 不等于操作符重载
 * 
 * 当两个选择不相等时返回true
 * 
 * @param other 要比较的另一个选择对象
 * @return 如果两个选择不同返回true
 */
bool Selection::operator!=(const Selection &other) const { return !(*this == other); }

/**
 * @brief 检查选择是否有效
 * 
 * 检查选择的起始位置和结束位置是否有效（段落索引大于等于0）
 * 
 * @return 如果选择有效返回true
 */
bool Selection::isValid() const {
    return m_start.paragraphIndex >= 0 && m_end.paragraphIndex >= 0;
}

/**
 * @brief 检查位置是否在选择区域内
 * 
 * 检查给定位置是否在选择区域内，使用规范化的起始和结束位置进行比较
 * 
 * @param pos 要检查的位置
 * @return 如果位置在选择区域内返回true
 */
bool Selection::contains(const Position &pos) const {
    Position normStart = normalizedStart();
    Position normEnd = normalizedEnd();
    
    // 检查位置是否在规范化的起始位置之后，且在规范化的结束位置之前
    return pos >= normStart && pos <= normEnd;
}