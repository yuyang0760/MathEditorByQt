// ============================================================================
// TextRun.h
// 文本片段类头文件
// 定义具有统一格式的文本片段，支持样式ID和直接格式覆盖
// ============================================================================

#ifndef TEXTRUN_H
#define TEXTRUN_H

#include "Format.h"
#include <QString>

class StyleManager; // 前置声明

/**
 * @brief 文本片段类
 * 
 * TextRun类表示具有统一格式的文本片段，是文档内容的基本单元。
 * 它包含文本内容、可选的样式ID和可选的直接格式覆盖。
 * 最终显示格式由样式ID决定的样式基础格式与直接格式覆盖合并而成。
 */
class TextRun {
public:
    /**
     * @brief 默认构造函数
     * 创建空的文本片段，无样式，直接格式为空
     */
    TextRun();
    
    /**
     * @brief 带文本和直接格式的构造函数（传统用法）
     * @param text 文本内容
     * @param format 直接格式
     */
    TextRun(const QString &text, const Format &format = Format());

    /**
     * @brief 带文本、样式ID和直接格式的构造函数
     * @param text 文本内容
     * @param styleId 样式ID
     * @param directFormat 直接格式（可选）
     */
    TextRun(const QString &text, const QString &styleId, const Format &directFormat = Format());

    /**
     * @brief 获取文本内容
     * @return 当前文本内容
     */
    QString text() const;
    
    /**
     * @brief 设置文本内容
     * @param text 新的文本内容
     */
    void setText(const QString &text);
    
    // 原有方法
    Format format() const; // 注意：此方法返回的是直接格式？还是最终格式？为了兼容，我们保留返回直接格式，但新增最终格式方法。
    void setFormat(const Format &format); // 设置直接格式
    void insert(int position, const QString &text);
    void remove(int position, int length);
    int length() const;

    // 新增方法
    QString styleId() const;
    void setStyleId(const QString &styleId);
    bool hasDirectFormat() const;
    Format directFormat() const;
    void setDirectFormat(const Format &format);
    void clearDirectFormat();

    /**
     * @brief 获取最终格式（结合样式和直接格式）
     * @param styleMgr 样式管理器指针
     * @return 合并后的格式
     */
    Format effectiveFormat(StyleManager *styleMgr) const;

private:
    QString m_text;
    QString m_styleId;          ///< 样式ID，空表示无样式（将使用段落样式或默认）
    Format m_directFormat;      ///< 直接格式覆盖，仅在需要覆盖样式时使用
    bool m_hasDirectFormat;     ///< 标记是否存在直接格式
};

#endif // TEXTRUN_H