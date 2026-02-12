// ============================================================================
// Paragraph.cpp
// 段落类的实现文件
// 表示文档中的一个段落，包含多个Run，提供段落的增删改查操作
// ============================================================================

#include "core/Paragraph.h"

/**
 * @brief 构造函数
 * 创建一个空段落
 */
Paragraph::Paragraph()
{
}

/**
 * @brief 获取段落文本
 * @return 段落文本
 */
QString Paragraph::text() const
{
    QString result;
    for (const Run &run : m_runs)
    {
        result += run.text();
    }
    return result;
}

/**
 * @brief 设置段落文本
 * @param text 段落文本
 */
void Paragraph::setText(const QString &text)
{
    m_runs.clear();
    if (!text.isEmpty())
    {
        m_runs.append(Run(text));
    }
}

/**
 * @brief 添加Run
 * @param run 要添加的Run
 */
void Paragraph::addRun(const Run &run)
{
    m_runs.append(run);
}

/**
 * @brief 在指定位置插入Run
 * @param position 插入位置
 * @param run 要插入的Run
 */
void Paragraph::insertRun(int position, const Run &run)
{
    if (position >= 0 && position <= m_runs.size())
    {
        m_runs.insert(position, run);
    }
}

/**
 * @brief 删除指定位置的Run
 * @param position 要删除的Run位置
 */
void Paragraph::removeRun(int position)
{
    if (position >= 0 && position < m_runs.size())
    {
        m_runs.removeAt(position);
    }
}

/**
 * @brief 获取指定位置的Run
 * @param position Run位置
 * @return Run对象
 */
Run Paragraph::run(int position) const
{
    if (position >= 0 && position < m_runs.size())
    {
        return m_runs[position];
    }
    return Run();
}

/**
 * @brief 获取Run数量
 * @return Run数量
 */
int Paragraph::runCount() const
{
    return m_runs.size();
}

/**
 * @brief 在指定位置插入文本
 * @param position 插入位置
 * @param text 要插入的文本
 * @param format 文本格式
 */
/**
 * @brief 在指定位置插入文本
 * @param position 插入位置（基于段落文本的字符位置）
 * @param text 要插入的文本内容
 * @param format 文本格式（字体、颜色等样式信息）
 * @note 如果插入位置在现有Run范围内，则在该Run中插入文本
 * @note 如果插入位置在段落末尾，则创建新Run并添加文本
 * @note 空文本将被忽略
 */
void Paragraph::insertText(int position, const QString &text, const Format &format) {
    if (text.isEmpty())
    return; // 空文本不做处理

    int currentPos = 0; // 当前遍历到的文本位置
    for (int i = 0; i < m_runs.size(); i++) {
        Run &run = m_runs[i];
        int runEnd = currentPos + run.length(); // 当前Run的结束位置

        // 如果插入位置在当前Run范围内
        if (position >= currentPos && position <= runEnd) {
            int runPos = position - currentPos; // 计算在Run内部的相对位置
            run.insert(runPos, text); // 在Run内部插入文本
            return;
        }
        currentPos = runEnd; // 更新当前遍历位置
    }

    // 如果位置在段落末尾，添加新的Run
    m_runs.append(Run(text, format));
}

/**
 * @brief 删除指定位置的文本
 * @param position 删除位置
 * @param length 删除长度
 */
void Paragraph::removeText(int position, int length)
{
    if (length <= 0)
        return;
    
    int currentPos = 0;
    for (int i = 0; i < m_runs.size() && length > 0; i++)
    {
        Run &run = m_runs[i];
        int runEnd = currentPos + run.length();
        
        if (position < runEnd)
        {
            int runPos = position - currentPos;
            int runLength = qMin(length, runEnd - position);
            run.remove(runPos, runLength);
            length -= runLength;
            position += runLength;
            
            // 如果Run为空，移除它
            if (run.length() == 0)
            {
                m_runs.removeAt(i);
                i--;
            }
        }
        currentPos = runEnd;
    }
}

/**
 * @brief 获取段落长度
 * @return 段落长度
 */
int Paragraph::length() const
{
    int totalLength = 0;
    for (const Run &run : m_runs)
    {
        totalLength += run.length();
    }
    return totalLength;
}
