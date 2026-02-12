// ============================================================================
// Document.h
// 文档类的头文件
// 表示整个文档，包含多个段落，提供文档的增删改查操作
// ============================================================================

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Paragraph.h"
#include <QVector>
#include <QString>

/**
 * @class Document
 * @brief 文档类
 * 
 * 表示整个文档，包含多个段落，提供文档的增删改查操作。
 * 是文档模型的核心类，管理文档的所有内容。
 */
class Document
{
public:
    /**
     * @brief 构造函数
     * 创建一个空文档
     */
    Document();
    
    /**
     * @brief 添加段落
     * @param paragraph 要添加的段落
     */
    void addParagraph(const Paragraph &paragraph);
    
    /**
     * @brief 在指定位置插入段落
     * @param position 插入位置
     * @param paragraph 要插入的段落
     */
    void insertParagraph(int position, const Paragraph &paragraph);
    
    /**
     * @brief 删除指定位置的段落
     * @param position 要删除的段落位置
     */
    void removeParagraph(int position);
    
    /**
     * @brief 获取指定位置的段落
     * @param position 段落位置
     * @return 段落对象
     */
    Paragraph paragraph(int position) const;
    
    /**
     * @brief 获取段落数量
     * @return 段落数量
     */
    int paragraphCount() const;
    
    /**
     * @brief 在指定位置插入文本
     * @param paragraphIndex 段落索引
     * @param position 插入位置
     * @param text 要插入的文本
     * @param format 文本格式，默认为默认格式
     */
    void insertText(int paragraphIndex, int position, const QString &text, const Format &format = Format());
    
    /**
     * @brief 删除指定位置的文本
     * @param paragraphIndex 段落索引
     * @param position 删除位置
     * @param length 删除长度
     */
    void removeText(int paragraphIndex, int position, int length);
    
    /**
     * @brief 获取整个文档的文本
     * @return 文档文本
     */
    QString text() const;
    
    /**
     * @brief 清空文档
     */
    void clear();
    
private:
    /**
     * @brief 段落列表
     */
    QVector<Paragraph> m_paragraphs;
};

#endif // DOCUMENT_H
