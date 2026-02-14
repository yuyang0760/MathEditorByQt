// ============================================================================
// TextRun.h
// 文本片段类头文件
// 定义具有统一格式的文本片段
// ============================================================================

#ifndef TEXTRUN_H
#define TEXTRUN_H

#include "Format.h"
#include <QString>

/**
 * @brief 文本片段类
 * 
 * TextRun类表示具有统一格式的文本片段，是文档内容的基本单元。
 * 它封装了文本内容和格式信息，支持文本的插入、删除和长度计算。
 * 
 * 主要功能：
 * - 文本内容管理
 * - 格式信息存储
 * - 文本编辑操作（插入、删除）
 * - 长度计算
 */
class TextRun {
public:
    /**
     * @brief 默认构造函数
     * 创建空的文本片段
     */
    TextRun();
    
    /**
     * @brief 带参数构造函数
     * @param text 文本内容
     * @param format 文本格式（默认为默认格式）
     */
    TextRun(const QString &text, const Format &format = Format());

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
     * @brief 获取格式信息
     * @return 当前格式对象
     */
    Format format() const;
    
    /**
     * @brief 设置格式信息
     * @param format 新的格式对象
     */
    void setFormat(const Format &format);

    /**
     * @brief 在指定位置插入文本
     * @param position 插入位置（从0开始）
     * @param text 要插入的文本
     */
    void insert(int position, const QString &text);
    
    /**
     * @brief 删除指定位置的文本
     * @param position 删除起始位置
     * @param length 要删除的字符数
     */
    void remove(int position, int length);
    
    /**
     * @brief 获取文本长度
     * @return 文本字符数
     */
    int length() const;

private:
    QString m_text;    ///< 文本内容
    Format m_format;   ///< 文本格式
};

#endif // TEXTRUN_H