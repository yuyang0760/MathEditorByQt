// ============================================================================
// Document.cpp
// 文档类实现文件
// 实现文档的基本操作，包括段落管理和文档维护
// ============================================================================

#include "core/Document.h"

/**
 * @brief Document构造函数
 * 初始化空文档，创建空的段落容器
 */
Document::Document() {}

/**
 * @brief 在文档末尾添加段落
 * 将指定段落追加到文档末尾
 * @param paragraph 要添加的段落对象
 */
void Document::addParagraph(const Paragraph &paragraph) { m_paragraphs.append(paragraph); }

/**
 * @brief 在指定位置插入段落
 * 在指定索引位置插入段落，原位置及后续段落向后移动
 * @param index 插入位置的索引
 * @param paragraph 要插入的段落对象
 */
void Document::insertParagraph(int index, const Paragraph &paragraph) { m_paragraphs.insert(index, paragraph); }

/**
 * @brief 删除指定位置的段落
 * 移除指定索引位置的段落，后续段落向前移动填补空位
 * @param index 要删除的段落索引
 */
void Document::removeParagraph(int index) { m_paragraphs.removeAt(index); }

/**
 * @brief 获取指定位置的段落（常量引用）
 * 返回指定索引位置段落的常量引用，不允许可修改访问
 * @param index 段落索引
 * @return 指定段落的常量引用
 */
Paragraph Document::paragraph(int index) const { return m_paragraphs[index]; }

/**
 * @brief 获取指定位置的段落（可修改引用）
 * 返回指定索引位置段落的可修改引用，允许直接修改段落内容
 * @param index 段落索引
 * @return 指定段落的可修改引用
 */
Paragraph &Document::paragraph(int index) { return m_paragraphs[index]; }

/**
 * @brief 获取文档中的段落数量
 * 返回当前文档中包含的段落总数
 * @return 段落总数
 */
int Document::paragraphCount() const { return m_paragraphs.size(); }

/**
 * @brief 清空文档
 * 移除文档中的所有段落，使文档变为空文档
 */
void Document::clear() { m_paragraphs.clear(); }