/*! \file Paragraph.h
    \brief 段落类头文件
    \author 项目开发者
    \date 2026-02-14
    
    \details 定义段落的数据结构和相关操作接口
*/

#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include <QVariant> // 用于存储 TextRun 或 MathObject
#include <QList>

class Format;
class MathObject;
class TextRun;

/*! \class Paragraph
    \brief 段落类
    
    Paragraph类代表文档中的一个段落，由多个内容项(Item)组成。
    每个项可以是文本片段(TextRun)或数学对象(MathObject)。
    支持混合内容类型，为富文本编辑提供基础。
    
    \section paragraph_features 主要功能
    - 管理段落内的内容项
    - 添加不同类型的内容
    - 获取纯文本表示（用于调试）
    
    \section paragraph_usage 使用示例
    \code
    Paragraph para;
    Format fmt(QFont("Arial", 12));
    para.appendText("Hello World", fmt);
    \endcode
*/
class Paragraph {
public:
    /*! \enum ItemType
        \brief 内容项类型枚举
        
        定义段落中可能包含的内容类型
    */
    enum ItemType { 
        TextRunItem,    ///< 文本片段项
        MathObjectItem  ///< 数学对象项
    };

    /*! \struct Item
        \brief 内容项结构体
        
        表示段落中的单个内容项
    */
    struct Item {
        ItemType type;      ///< 项的类型
        QVariant data;      ///< 项的数据：TextRun 或 MathObject
    };

    /*! \brief 构造函数
        
        创建空段落
    */
    Paragraph();

    /*! \brief 向段落末尾添加文本
        
        \param text 要添加的文本内容
        \param format 文本格式
    */
    void appendText(const QString &text, const Format &format);
    
    /*! \brief 向段落末尾添加数学对象
        
        \param obj 要添加的数学对象
    */
    void appendMathObject(const MathObject &obj);

    /*! \brief 获取段落中的项数量
        
        \return 项的总数
    */
    int itemCount() const;
    
    /*! \brief 获取指定位置的项（常量引用）
        
        \param index 项的索引
        \return 指定项的常量引用
    */
    const Item &itemAt(int index) const;
    
    /*! \brief 获取指定位置的项（可修改引用）
        
        \param index 项的索引
        \return 指定项的可修改引用
    */
    Item &itemAt(int index);
    
    /*! \brief 在指定位置插入项
        
        \param index 插入位置
        \param item 要插入的项
    */
    void insertItem(int index, const Item &item);

    /*! \brief 获取段落的纯文本表示
        
        \return 段落中所有文本内容的拼接字符串
        \note 主要用于调试目的
    */
    QString plainText() const;

private:
    QList<Item> m_items;  ///< 存储段落内容项的列表
};

#endif // PARAGRAPH_H