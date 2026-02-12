#include "core/Paragraph.h"

Paragraph::Paragraph()
{
}

QString Paragraph::text() const
{
    QString result;
    for (const Run &run : m_runs)
    {
        result += run.text();
    }
    return result;
}

void Paragraph::setText(const QString &text)
{
    m_runs.clear();
    if (!text.isEmpty())
    {
        m_runs.append(Run(text));
    }
}

void Paragraph::addRun(const Run &run)
{
    m_runs.append(run);
}

void Paragraph::insertRun(int position, const Run &run)
{
    if (position >= 0 && position <= m_runs.size())
    {
        m_runs.insert(position, run);
    }
}

void Paragraph::removeRun(int position)
{
    if (position >= 0 && position < m_runs.size())
    {
        m_runs.removeAt(position);
    }
}

Run Paragraph::run(int position) const
{
    if (position >= 0 && position < m_runs.size())
    {
        return m_runs[position];
    }
    return Run();
}

int Paragraph::runCount() const
{
    return m_runs.size();
}

void Paragraph::insertText(int position, const QString &text, const Format &format)
{
    if (text.isEmpty())
        return;
    
    int currentPos = 0;
    for (int i = 0; i < m_runs.size(); i++)
    {
        Run &run = m_runs[i];
        int runEnd = currentPos + run.length();
        
        if (position >= currentPos && position <= runEnd)
        {
            int runPos = position - currentPos;
            run.insert(runPos, text);
            return;
        }
        currentPos = runEnd;
    }
    
    // 如果位置在段落末尾，添加新的Run
    m_runs.append(Run(text, format));
}

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

int Paragraph::length() const
{
    int totalLength = 0;
    for (const Run &run : m_runs)
    {
        totalLength += run.length();
    }
    return totalLength;
}
