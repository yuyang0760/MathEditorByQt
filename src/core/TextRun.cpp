// ============================================================================
// TextRun.cpp
// 文本片段类实现文件
// 实现文本片段的具体操作功能
// ============================================================================

#include "core/TextRun.h"

/**
 * @brief 默认构造函数
 * 创建空的文本片段
 */
TextRun::TextRun() {}

/**
 * @brief 带参数构造函数
 * @param text 文本内容
 * @param format 文本格式
 */
TextRun::TextRun(const QString &text, const Format &format) : m_text(text), m_format(format) {}

/**
 * @brief 获取文本内容
 * @return 当前文本内容
 */
QString TextRun::text() const { return m_text; }

/**
 * @brief 设置文本内容
 * @param text 新的文本内容
 */
void TextRun::setText(const QString &text) { m_text = text; }

/**
 * @brief 获取格式信息
 * @return 当前格式对象
 */
Format TextRun::format() const { return m_format; }

/**
 * @brief 设置格式信息
 * @param format 新的格式对象
 */
void TextRun::setFormat(const Format &format) { m_format = format; }

/**
 * @brief 在指定位置插入文本
 * @param position 插入位置（从0开始）
 * @param text 要插入的文本
 */
void TextRun::insert(int position, const QString &text) { m_text.insert(position, text); }

/**
 * @brief 删除指定位置的文本
 * @param position 删除起始位置
 * @param length 要删除的字符数
 */
void TextRun::remove(int position, int length) { m_text.remove(position, length); }

/**
 * @brief 获取文本长度
 * @return 文本字符数
 */
int TextRun::length() const { return m_text.length(); }