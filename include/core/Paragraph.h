#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include "Run.h"
#include <QVector>

class Paragraph
{
public:
    Paragraph();
    
    // 获取和设置段落文本
    QString text() const;
    void setText(const QString &text);
    
    // Run操作
    void addRun(const Run &run);
    void insertRun(int position, const Run &run);
    void removeRun(int position);
    Run run(int position) const;
    int runCount() const;
    
    // 文本操作
    void insertText(int position, const QString &text, const Format &format = Format());
    void removeText(int position, int length);
    
    // 获取长度
    int length() const;
    
private:
    QVector<Run> m_runs;
};

#endif // PARAGRAPH_H
