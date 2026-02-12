#ifndef RUN_H
#define RUN_H

#include "Format.h"
#include <QString>

class Run
{
public:
    Run();
    Run(const QString &text, const Format &format = Format());
    
    // 获取和设置文本内容
    QString text() const;
    void setText(const QString &text);
    
    // 获取和设置格式
    Format format() const;
    void setFormat(const Format &format);
    
    // 文本操作
    void insert(int position, const QString &text);
    void remove(int position, int length);
    void replace(int position, int length, const QString &text);
    
    // 获取长度
    int length() const;
    
private:
    QString m_text;
    Format m_format;
};

#endif // RUN_H
