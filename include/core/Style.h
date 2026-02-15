// ============================================================================
// Style.h
// 样式类头文件
// 定义命名的格式集合，支持继承关系
// ============================================================================

#ifndef STYLE_H
#define STYLE_H

#include "Format.h"
#include <QString>

/**
 * @brief 样式类
 * 
 * Style表示一个命名的格式集合，可以应用于段落或文本片段。
 * 支持基于父样式的继承，子样式会继承父样式的所有属性，并可覆盖。
 */
class Style {
public:
    /**
     * @brief 构造函数
     * @param id 样式唯一标识符
     * @param name 样式名称
     * @param format 样式格式
     * @param parentId 父样式ID（默认空，表示无父样式）
     */
    Style(const QString &id = QString(),
          const QString &name = QString(),
          const Format &format = Format(),
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
     * @brief 获取样式格式
     */
    Format format() const;

    /**
     * @brief 设置样式格式
     */
    void setFormat(const Format &format);

    /**
     * @brief 获取父样式ID
     */
    QString parentId() const;

    /**
     * @brief 设置父样式ID
     */
    void setParentId(const QString &parentId);

    /**
     * @brief 判断是否相等
     */
    bool operator==(const Style &other) const;
    bool operator!=(const Style &other) const;

private:
    QString m_id;           ///< 样式唯一标识符（例如 "style_paragraph_normal"）
    QString m_name;         ///< 样式显示名称（例如 "正文"）
    Format m_format;        ///< 样式定义的格式
    QString m_parentId;     ///< 父样式ID（用于继承）
};

#endif // STYLE_H