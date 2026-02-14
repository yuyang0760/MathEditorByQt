/*! \file Document.h
    \brief 文档类头文件
    \author 项目开发者
    \date 2026-02-14
    
    \details 提供文档级别的操作接口，管理段落集合
*/

#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Paragraph.h"
#include <QVector>

/*! \class Document
    \brief 文档类
    
    Document类是整个文档系统的根节点，负责管理文档中的所有段落。
    它提供了段落级别的增删改查操作，并维护文档的整体结构。
    
    \section document_features 主要功能
    - 段落管理（添加、插入、删除、获取）
    - 文档清空
    - 段落数量统计
    
    \section document_usage 使用示例
    \code
    Document doc;
    Paragraph para;
    para.appendText("Hello World", Format());
    doc.addParagraph(para);
    \endcode
*/
class Document {
public:
    /*! \brief 构造函数
        
        初始化空文档，创建空的段落容器。
    */
    Document();

    /*! \brief 在文档末尾添加段落
        
        \param paragraph 要添加的段落对象
    */
    void addParagraph(const Paragraph &paragraph);
    
    /*! \brief 在指定位置插入段落
        
        \param index 插入位置索引
        \param paragraph 要插入的段落对象
    */
    void insertParagraph(int index, const Paragraph &paragraph);
    
    /*! \brief 删除指定位置的段落
        
        \param index 要删除的段落索引
    */
    void removeParagraph(int index);
    
    /*! \brief 获取指定位置的段落（常量引用）
        
        \param index 段落索引
        \return 指定段落的常量引用
    */
    Paragraph paragraph(int index) const;
    
    /*! \brief 获取指定位置的段落（可修改引用）
        
        \param index 段落索引
        \return 指定段落的可修改引用
    */
    Paragraph &paragraph(int index);
    
    /*! \brief 获取文档中的段落数量
        
        \return 段落总数
    */
    int paragraphCount() const;

    /*! \brief 清空文档中的所有段落
    */
    void clear();

private:
    QVector<Paragraph> m_paragraphs;  ///< 存储段落的容器
};

#endif // DOCUMENT_H