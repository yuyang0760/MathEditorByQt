// ============================================================================
// Format.cpp
// 格式类的实现文件
// 表示文本的格式，包含字体、颜色等属性
// ============================================================================

#include "core/Format.h"

/**
 * @brief 构造函数
 * 创建一个默认格式
 */
Format::Format()
{
}

/**
 * @brief 构造函数
 * @param font 字体
 * @param color 颜色
 */
Format::Format(const QFont &font, const QColor &color)
    : m_font(font), m_color(color)
{
}

/**
 * @brief 获取字体
 * @return 字体
 */
QFont Format::font() const
{
    return m_font;
}

/**
 * @brief 设置字体
 * @param font 字体
 */
void Format::setFont(const QFont &font)
{
    m_font = font;
}

/**
 * @brief 获取颜色
 * @return 颜色
 */
QColor Format::color() const
{
    return m_color;
}

/**
 * @brief 设置颜色
 * @param color 颜色
 */
void Format::setColor(const QColor &color)
{
    m_color = color;
}

/**
 * @brief 检查是否为粗体
 * @return 是否为粗体
 */
bool Format::bold() const
{
    return m_font.bold();
}

/**
 * @brief 设置粗体
 * @param bold 是否为粗体
 */
void Format::setBold(bool bold)
{
    m_font.setBold(bold);
}

/**
 * @brief 检查是否为斜体
 * @return 是否为斜体
 */
bool Format::italic() const
{
    return m_font.italic();
}

/**
 * @brief 设置斜体
 * @param italic 是否为斜体
 */
void Format::setItalic(bool italic)
{
    m_font.setItalic(italic);
}

/**
 * @brief 检查是否有下划线
 * @return 是否有下划线
 */
bool Format::underline() const
{
    return m_font.underline();
}

/**
 * @brief 设置下划线
 * @param underline 是否有下划线
 */
void Format::setUnderline(bool underline)
{
    m_font.setUnderline(underline);
}

/**
 * @brief 比较两个格式是否相等
 * @param other 另一个格式
 * @return 是否相等
 */
bool Format::operator==(const Format &other) const
{
    return m_font == other.m_font && m_color == other.m_color;
}

/**
 * @brief 比较两个格式是否不相等
 * @param other 另一个格式
 * @return 是否不相等
 */
bool Format::operator!=(const Format &other) const
{
    return !(*this == other);
}
