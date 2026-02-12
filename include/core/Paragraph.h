// ============================================================================
// Paragraph.h
// 段落类的头文件
// 表示文档中的一个段落，包含多个Run，提供段落的增删改查操作
// ============================================================================

#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include "Run.h"
#include <QVector>
#include <QString>

class Format;

/**
 * @class Paragraph
 * @brief 段落类
 * 
 * 表示文档中的一个段落，包含多个Run，提供段落的增删改查操作。
 * 段落是文档的基本组成单位，由一个或多个具有相同格式的文本片段（Run）组成。
 */
class Paragraph
{
public:
    /**
     * @brief 构造函数
     * 创建一个空段落
     */
    Paragraph();
    
    /**
     * @brief 获取段落文本
     * @return 段落文本
     */
    QString text() const;
    
    /**
     * @brief 设置段落文本
     * @param text 段落文本
     */
    void setText(const QString &text);
    
    /**
     * @brief 添加Run
     * @param run 要添加的Run
     */
    void addRun(const Run &run);
    
    /**
     * @brief 在指定位置插入Run
     * @param position 插入位置
     * @param run 要插入的Run
     */
    void insertRun(int position, const Run &run);
    
    /**
     * @brief 删除指定位置的Run
     * @param position 要删除的Run位置
     */
    void removeRun(int position);
    
    /**
     * @brief 获取指定位置的Run
     * @param position Run位置
     * @return Run对象
     */
    Run run(int position) const;
    
    /**
     * @brief 获取Run数量
     * @return Run数量
     */
    int runCount() const;
    
    /**
     * @brief 在指定位置插入文本
     * @param position 插入位置
     * @param text 要插入的文本
     * @param format 文本格式，默认为默认格式
     */
    void insertText(int position, const QString &text, const Format &format = Format());
    
    /**
     * @brief 删除指定位置的文本
     * @param position 删除位置
     * @param length 删除长度
     */
    void removeText(int position, int length);
    
    /**
     * @brief 获取段落长度
     * @return 段落长度
     */
    int length() const;
    
private:
    /**
     * @brief Run列表
     */
    QVector<Run> m_runs;
};

#endif // PARAGRAPH_H
