// ============================================================================
// MathObject.cpp
// 数学对象类实现文件
// 实现数学公式对象的基本功能
// ============================================================================

#include "core/MathObject.h"

/**
 * @brief 构造函数
 * 初始化数学对象，设置类型
 * @param type 数学对象类型
 */
MathObject::MathObject(Type type) : m_type(type) {}

/**
 * @brief 获取数学对象类型
 * @return 当前数学对象类型
 */
MathObject::Type MathObject::type() const { return m_type; }

/**
 * @brief 设置LaTeX公式
 * @param latex LaTeX格式的公式字符串
 */
void MathObject::setLatex(const QString &latex) { m_latex = latex; }

/**
 * @brief 获取LaTeX公式
 * @return LaTeX格式的公式字符串
 */
QString MathObject::latex() const { return m_latex; }