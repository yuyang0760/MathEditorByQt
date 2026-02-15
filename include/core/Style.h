// ============================================================================
// Style.h
// 样式类头文件
// 定义命名的格式集合，支持继承关系
// ============================================================================

#ifndef STYLE_H
#define STYLE_H

#include "CharacterFormat.h"
#include "ParagraphFormat.h"
#include <QString>

/**
 * @brief 样式类型枚举
 */
enum class StyleType {
    Paragraph,  ///< 段落样式
    Character   ///< 字符样式
};

/**
 * @brief 样式分类枚举
 */
enum class StyleCategory {
    All,        ///< 所有样式
    BuiltIn,    ///< 内置样式
    Custom,     ///< 自定义样式
    Paragraph,  ///< 段落样式
    Character,  ///< 字符样式
    Heading,    ///< 标题样式
    List,       ///< 列表样式
    Other       ///< 其他样式
};

/**
 * @brief 样式类
 * 
 * Style表示一个命名的格式集合，可以应用于段落或文本片段。
 * 支持基于父样式的继承，子样式会继承父样式的所有属性，并可覆盖。
 * 样式分为段落样式和字符样式两种类型。
 */
class Style {
public:
    /**
     * @brief 默认构造函数
     */
    Style();

    /**
     * @brief 构造函数
     * @param id 样式唯一标识符
     * @param name 样式名称
     * @param type 样式类型
     * @param charFormat 字符格式（字符样式必需，段落样式可选）
     * @param paraFormat 段落格式（段落样式必需）
     * @param parentId 父样式ID（默认空，表示无父样式）
     */
    Style(const QString &id,
          const QString &name,
          StyleType type = StyleType::Paragraph,
          const CharacterFormat &charFormat = CharacterFormat(),
          const ParagraphFormat &paraFormat = ParagraphFormat(),
          const QString &parentId = QString());

    /**
     * @brief 获取样式ID
     */
    QString id() const;

    /**
     * @brief 设置样式ID
     */
    void setId(const QString &id);

    /**
     * @brief 获取样式名称
     */
    QString name() const;

    /**
     * @brief 设置样式名称
     */
    void setName(const QString &name);

    /**
     * @brief 获取样式类型
     */
    StyleType type() const;

    /**
     * @brief 设置样式类型
     */
    void setType(StyleType type);

    /**
     * @brief 获取样式分类
     */
    StyleCategory category() const;

    /**
     * @brief 设置样式分类
     */
    void setCategory(StyleCategory category);

    /**
     * @brief 获取样式描述
     */
    QString description() const;

    /**
     * @brief 设置样式描述
     */
    void setDescription(const QString &description);

    /**
     * @brief 是否为内置样式
     */
    bool isBuiltIn() const;

    /**
     * @brief 设置是否为内置样式
     */
    void setBuiltIn(bool builtIn);

    /**
     * @brief 是否隐藏
     */
    bool isHidden() const;

    /**
     * @brief 设置是否隐藏
     */
    void setHidden(bool hidden);

    /**
     * @brief 获取显示优先级
     */
    int priority() const;

    /**
     * @brief 设置显示优先级
     */
    void setPriority(int priority);

    /**
     * @brief 获取下一个段落的默认样式ID
     */
    QString nextStyleId() const;

    /**
     * @brief 设置下一个段落的默认样式ID
     */
    void setNextStyleId(const QString &nextStyleId);

    /**
     * @brief 获取字符格式
     */
    CharacterFormat characterFormat() const;

    /**
     * @brief 设置字符格式
     */
    void setCharacterFormat(const CharacterFormat &format);

    /**
     * @brief 获取段落格式
     */
    ParagraphFormat paragraphFormat() const;

    /**
     * @brief 设置段落格式
     */
    void setParagraphFormat(const ParagraphFormat &format);

    /**
     * @brief 获取父样式ID
     */
    QString parentId() const;

    /**
     * @brief 设置父样式ID
     */
    void setParentId(const QString &parentId);

    /**
     * @brief 检查是否有父样式
     */
    bool hasParent() const;

    /**
     * @brief 检查是否可以删除
     */
    bool canDelete() const;

    /**
     * @brief 检查是否可以修改
     */
    bool canModify() const;

    /**
     * @brief 深拷贝
     */
    Style clone() const;

    /**
     * @brief 判断是否相等
     */
    bool operator==(const Style &other) const;
    bool operator!=(const Style &other) const;

private:
    QString m_id;               ///< 样式唯一标识符
    QString m_name;             ///< 样式显示名称
    StyleType m_type;           ///< 样式类型
    StyleCategory m_category;   ///< 样式分类
    QString m_description;      ///< 样式描述
    bool m_isBuiltIn;           ///< 是否为内置样式
    bool m_isHidden;            ///< 是否隐藏
    int m_priority;             ///< 显示优先级
    QString m_nextStyleId;      ///< 下一个段落的默认样式ID
    CharacterFormat m_charFormat;   ///< 字符格式
    ParagraphFormat m_paraFormat;   ///< 段落格式
    QString m_parentId;         ///< 父样式ID（用于继承）
};

#endif // STYLE_H
