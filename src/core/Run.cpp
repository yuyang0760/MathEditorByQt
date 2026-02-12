// ============================================================================
// Run.cpp
// Run类的实现文件
// 表示具有相同格式的文本片段
// ============================================================================

#include "core/Run.h"

/**
 * @brief 构造函数
 * 创建一个空Run
 */
Run::Run()
{
}

/**
 * @brief 构造函数
 * @param text 文本内容
 * @param format 文本格式
 */
Run::Run(const QString &text, const Format &format)
    : m_text(text), m_format(format)
{
}

/**
 * @brief 获取文本内容
 * @return 文本内容
 */
QString Run::text() const
{
    return m_text;
}

/**
 * @brief 设置文本内容
 * @param text 文本内容
 */
void Run::setText(const QString &text)
{
    m_text = text;
}

/**
 * @brief 获取格式
 * @return 格式
 */
Format Run::format() const
{
    return m_format;
}

/**
 * @brief 设置格式
 * @param format 格式
 */
void Run::setFormat(const Format &format)
{
    m_format = format;
}

/**
 * @brief 在指定位置插入文本
 * @param position 插入位置
 * @param text 要插入的文本
 */
void Run::insert(int position, const QString &text)
{
    m_text.insert(position, text);
}

/**
 * @brief 删除指定位置的文本
 * @param position 删除位置
 * @param length 删除长度
 */
void Run::remove(int position, int length)
{
    m_text.remove(position, length);
}

/**
 * @brief 替换指定位置的文本
 * @param position 替换位置
 * @param length 替换长度
 * @param text 替换的文本
 */
void Run::replace(int position, int length, const QString &text)
{
    m_text.replace(position, length, text);
}

/**
 * @brief 获取文本长度
 * @return 文本长度
 */
int Run::length() const
{
    return m_text.length();
}
