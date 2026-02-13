#ifndef TEXTRUN_H
#define TEXTRUN_H

#include "Format.h"
#include <QString>

class TextRun {
public:
    TextRun();
    TextRun(const QString &text, const Format &format = Format());

    QString text() const;
    void setText(const QString &text);
    Format format() const;
    void setFormat(const Format &format);

    void insert(int position, const QString &text);
    void remove(int position, int length);
    int length() const;

private:
    QString m_text;
    Format m_format;
};

#endif // TEXTRUN_H