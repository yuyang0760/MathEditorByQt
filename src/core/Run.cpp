#include "core/Run.h"

Run::Run()
{
}

Run::Run(const QString &text, const Format &format)
    : m_text(text), m_format(format)
{
}

QString Run::text() const
{
    return m_text;
}

void Run::setText(const QString &text)
{
    m_text = text;
}

Format Run::format() const
{
    return m_format;
}

void Run::setFormat(const Format &format)
{
    m_format = format;
}

void Run::insert(int position, const QString &text)
{
    m_text.insert(position, text);
}

void Run::remove(int position, int length)
{
    m_text.remove(position, length);
}

void Run::replace(int position, int length, const QString &text)
{
    m_text.replace(position, length, text);
}

int Run::length() const
{
    return m_text.length();
}
