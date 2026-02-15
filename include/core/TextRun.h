// ============================================================================
// TextRun.h
// 文本片段类头文件
// 定义具有统一格式的文本片段，支持样式ID和直接格式覆盖
// ============================================================================

#ifndef TEXTRUN_H
#define TEXTRUN_H

#include "CharacterFormat.h"
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
     * @brief 带文本和直接格式的构造函数
     * @param text 文本内容
     * @param charFormat 直接字符格式
     */
    TextRun(const QString &text, const CharacterFormat &charFormat = CharacterFormat());

    /**
     * @brief 带文本、样式ID和直接格式的构造函数
     * @param text 文本内容
     * @param styleId 样式ID
     * @param directCharFormat 直接字符格式（可选）
     */
    TextRun(const QString &text, const QString &styleId, const CharacterFormat &directCharFormat = CharacterFormat());

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
    
    /**
     * @brief 插入文本
     * @param position 插入位置
     * @param text 要插入的文本
     */
    void insert(int position, const QString &text);
    
    /**
     * @brief 删除文本
     * @param position 删除起始位置
     * @param length 删除长度
     */
    void remove(int position, int length);
    
    /**
     * @brief 获取文本长度
     * @return 文本长度
     */
    int length() const;

    /**
     * @brief 获取样式ID
     * @return 样式ID
     */
    QString styleId() const;
    
    /**
     * @brief 设置样式ID
     * @param styleId 样式ID
     */
    void setStyleId(const QString &styleId);
    
    /**
     * @brief 检查是否有直接格式
     * @return 如果有直接格式返回true
     */
    bool hasDirectFormat() const;
    
    /**
     * @brief 获取直接字符格式
     * @return 直接字符格式
     */
    CharacterFormat directFormat() const;
    
    /**
     * @brief 设置直接字符格式
     * @param format 直接字符格式
     */
    void setDirectFormat(const CharacterFormat &format);
    
    /**
     * @brief 清除直接格式
     */
    void clearDirectFormat();

    /**
     * @brief 获取最终格式（结合样式和直接格式）
     * @param styleMgr 样式管理器指针
     * @return 合并后的字符格式
     */
    CharacterFormat effectiveFormat(StyleManager *styleMgr) const;

private:
    QString m_text;
    QString m_styleId;          ///< 样式ID，空表示无样式（将使用段落样式或默认）
    CharacterFormat m_directFormat; ///< 直接字符格式覆盖
    bool m_hasDirectFormat;     ///< 标记是否存在直接格式
};

#endif // TEXTRUN_H
