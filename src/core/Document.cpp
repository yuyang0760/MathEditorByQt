// ============================================================================
// Document.cpp
// 文档类的实现文件
// 表示整个文档，包含多个段落，提供文档的增删改查操作
// ============================================================================

#include "core/Document.h"

/**
 * @brief 构造函数
 * 创建一个空文档
 */
Document::Document()
{
}

/**
 * @brief 添加段落
 * @param paragraph 要添加的段落
 */
void Document::addParagraph(const Paragraph &paragraph)
{
    m_paragraphs.append(paragraph);
}

/**
 * @brief 在指定位置插入段落
 * @param position 插入位置
 * @param paragraph 要插入的段落
 */
void Document::insertParagraph(int position, const Paragraph &paragraph)
{
    if (position >= 0 && position <= m_paragraphs.size())
    {
        m_paragraphs.insert(position, paragraph);
    }
}

/**
 * @brief 删除指定位置的段落
 * @param position 要删除的段落位置
 */
void Document::removeParagraph(int position)
{
    if (position >= 0 && position < m_paragraphs.size())
    {
        m_paragraphs.removeAt(position);
    }
}

/**
 * @brief 获取指定位置的段落
 * @param position 段落位置
 * @return 段落对象
 */
Paragraph Document::paragraph(int position) const
{
    if (position >= 0 && position < m_paragraphs.size())
    {
        return m_paragraphs[position];
    }
    return Paragraph();
}

/**
 * @brief 获取段落数量
 * @return 段落数量
 */
int Document::paragraphCount() const
{
    return m_paragraphs.size();
}

/**
 * @brief 在指定位置插入文本
 * @param paragraphIndex 段落索引
 * @param position 插入位置
 * @param text 要插入的文本
 * @param format 文本格式
 */
void Document::insertText(int paragraphIndex, int position, const QString &text, const Format &format)
{
    if (paragraphIndex >= 0 && paragraphIndex < m_paragraphs.size())
    {
        m_paragraphs[paragraphIndex].insertText(position, text, format);
    }
}

/**
 * @brief 删除指定位置的文本
 * @param paragraphIndex 段落索引
 * @param position 删除位置
 * @param length 删除长度
 */
void Document::removeText(int paragraphIndex, int position, int length)
{
    if (paragraphIndex >= 0 && paragraphIndex < m_paragraphs.size())
    {
        m_paragraphs[paragraphIndex].removeText(position, length);
    }
}

/**
 * @brief 获取整个文档的文本
 * @return 文档文本
 */
QString Document::text() const
{
    QString result;
    for (int i = 0; i < m_paragraphs.size(); i++)
    {
        result += m_paragraphs[i].text();
        if (i < m_paragraphs.size() - 1)
        {
            result += "\n";
        }
    }
    return result;
}

/**
 * @brief 清空文档
 */
void Document::clear()
{
    m_paragraphs.clear();
}
