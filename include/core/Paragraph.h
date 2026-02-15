// ============================================================================
// Paragraph.h
// 段落类头文件
// 定义段落的数据结构和相关操作接口
// ============================================================================

#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include <QVariant>
#include <QList>

#include "CharacterFormat.h"
#include "ParagraphFormat.h"

class MathObject;
class TextRun;
class StyleManager;

/**
 * @brief 段落类
 * 
 * Paragraph类代表文档中的一个段落，由多个内容项(Item)组成。
 * 每个项可以是文本片段(TextRun)或数学对象(MathObject)。
 */
class Paragraph {
public:
    /**
     * @brief 内容项类型枚举
     */
    enum ItemType { 
        TextRunItem,    ///< 文本片段项
        MathObjectItem  ///< 数学对象项
    };

    /**
     * @brief 内容项结构体
     */
    struct Item {
        ItemType type;      ///< 项的类型
        QVariant data;      ///< 项的数据：TextRun 或 MathObject
    };

    /**
     * @brief 构造函数
     */
    Paragraph();

    /**
     * @brief 向段落末尾添加文本
     * @param text 要添加的文本内容
     * @param charFormat 字符格式
     */
    void appendText(const QString &text, const CharacterFormat &charFormat);
    
    /**
     * @brief 向段落末尾添加文本（带样式）
     * @param text 要添加的文本内容
     * @param styleId 样式ID
     * @param directCharFormat 直接字符格式（可选）
     */
    void appendText(const QString &text, const QString &styleId, const CharacterFormat &directCharFormat = CharacterFormat());
    
    /**
     * @brief 向段落末尾添加数学对象
     * @param obj 要添加的数学对象
     */
    void appendMathObject(const MathObject &obj);

    /**
     * @brief 获取段落中的项数量
     * @return 项的总数
     */
    int itemCount() const;
    
    /**
     * @brief 获取指定位置的项（常量引用）
     * @param index 项的索引
     * @return 指定项的常量引用
     */
    const Item &itemAt(int index) const;
    
    /**
     * @brief 获取指定位置的项（可修改引用）
     * @param index 项的索引
     * @return 指定项的可修改引用
     */
    Item &itemAt(int index);
    
    /**
     * @brief 在指定位置插入项
     * @param index 插入位置
     * @param item 要插入的项
     */
    void insertItem(int index, const Item &item);

    /**
     * @brief 替换指定范围的项
     * @param index 起始位置
     * @param count 要替换的项数量
     * @param newItems 新的项列表
     */
    void replaceItems(int index, int count, const QList<Item> &newItems);

    /**
     * @brief 获取段落的纯文本表示
     * @return 段落中所有文本内容的拼接字符串
     */
    QString plainText() const;

    // 段落样式相关
    QString styleId() const;
    void setStyleId(const QString &styleId);
    
    bool hasDirectFormat() const;
    ParagraphFormat directFormat() const;
    void setDirectFormat(const ParagraphFormat &format);
    void clearDirectFormat();
    
    /**
     * @brief 获取段落的最终字符格式（用于默认文本）
     * @param styleMgr 样式管理器
     * @return 最终字符格式
     */
    CharacterFormat effectiveCharacterFormat(StyleManager *styleMgr) const;
    
    /**
     * @brief 获取段落的最终段落格式
     * @param styleMgr 样式管理器
     * @return 最终段落格式
     */
    ParagraphFormat effectiveParagraphFormat(StyleManager *styleMgr) const;

private:
    QList<Item> m_items;
    QString m_styleId;           ///< 段落样式ID
    ParagraphFormat m_directFormat; ///< 段落直接格式
    bool m_hasDirectFormat;      ///< 是否有直接格式
};

#endif // PARAGRAPH_H
