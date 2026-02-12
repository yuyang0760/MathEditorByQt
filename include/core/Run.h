// ============================================================================
// Run.h
// Run类的头文件
// 表示具有相同格式的文本片段
// ============================================================================

#ifndef RUN_H
#define RUN_H

#include "Format.h"
#include <QString>

/**
 * @class Run
 * @brief Run类
 * 
 * 表示具有相同格式的文本片段。
 * 是段落的基本组成单位，包含文本内容和格式信息。
 */
class Run
{
public:
    /**
     * @brief 构造函数
     * 创建一个空Run
     */
    Run();
    
    /**
     * @brief 构造函数
     * @param text 文本内容
     * @param format 文本格式，默认为默认格式
     */
    Run(const QString &text, const Format &format = Format());
    
    /**
     * @brief 获取文本内容
     * @return 文本内容
     */
    QString text() const;
    
    /**
     * @brief 设置文本内容
     * @param text 文本内容
     */
    void setText(const QString &text);
    
    /**
     * @brief 获取格式
     * @return 格式
     */
    Format format() const;
    
    /**
     * @brief 设置格式
     * @param format 格式
     */
    void setFormat(const Format &format);
    
    /**
     * @brief 在指定位置插入文本
     * @param position 插入位置
     * @param text 要插入的文本
     */
    void insert(int position, const QString &text);
    
    /**
     * @brief 删除指定位置的文本
     * @param position 删除位置
     * @param length 删除长度
     */
    void remove(int position, int length);
    
    /**
     * @brief 替换指定位置的文本
     * @param position 替换位置
     * @param length 替换长度
     * @param text 替换的文本
     */
    void replace(int position, int length, const QString &text);
    
    /**
     * @brief 获取文本长度
     * @return 文本长度
     */
    int length() const;
    
private:
    /**
     * @brief 文本内容
     */
    QString m_text;
    
    /**
     * @brief 文本格式
     */
    Format m_format;
};

#endif // RUN_H
