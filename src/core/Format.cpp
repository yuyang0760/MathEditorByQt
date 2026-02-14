// ============================================================================
// Format.cpp
// 格式类实现文件
// 实现文本格式属性的具体功能
// ============================================================================

#include "core/Format.h"

/**
 * @brief 默认构造函数
 * 创建默认格式对象
 */
Format::Format() {}

/**
 * @brief 带参数构造函数
 * @param font 字体对象
 * @param color 颜色对象
 */
Format::Format(const QFont &font, const QColor &color) : m_font(font), m_color(color) {}

/**
 * @brief 获取字体
 * @return 当前字体对象
 */
QFont Format::font() const { return m_font; }

/**
 * @brief 设置字体
 * @param font 新的字体对象
 */
void Format::setFont(const QFont &font) { m_font = font; }

/**
 * @brief 获取颜色
 * @return 当前颜色对象
 */
QColor Format::color() const { return m_color; }

/**
 * @brief 设置颜色
 * @param color 新的颜色对象
 */
void Format::setColor(const QColor &color) { m_color = color; }

/**
 * @brief 检查是否为粗体
 * @return 如果是粗体返回true
 */
bool Format::bold() const { return m_font.bold(); }

/**
 * @brief 设置粗体样式
 * @param bold 是否设置为粗体
 */
void Format::setBold(bool bold) { m_font.setBold(bold); }

/**
 * @brief 检查是否为斜体
 * @return 如果是斜体返回true
 */
bool Format::italic() const { return m_font.italic(); }

/**
 * @brief 设置斜体样式
 * @param italic 是否设置为斜体
 */
void Format::setItalic(bool italic) { m_font.setItalic(italic); }

/**
 * @brief 检查是否有下划线
 * @return 如果有下划线返回true
 */
bool Format::underline() const { return m_font.underline(); }

/**
 * @brief 设置下划线样式
 * @param underline 是否设置下划线
 */
void Format::setUnderline(bool underline) { m_font.setUnderline(underline); }

/**
 * @brief 等于操作符重载
 * @param other 要比较的格式对象
 * @return 如果两个格式相同返回true
 */
bool Format::operator==(const Format &other) const {
    return m_font == other.m_font && m_color == other.m_color;
}

/**
 * @brief 不等于操作符重载
 * @param other 要比较的格式对象
 * @return 如果两个格式不同返回true
 */
bool Format::operator!=(const Format &other) const { return !(*this == other); }