#include "core/Format.h"

Format::Format()
{
}

Format::Format(const QFont &font, const QColor &color)
    : m_font(font), m_color(color)
{
}

QFont Format::font() const
{
    return m_font;
}

void Format::setFont(const QFont &font)
{
    m_font = font;
}

QColor Format::color() const
{
    return m_color;
}

void Format::setColor(const QColor &color)
{
    m_color = color;
}

bool Format::bold() const
{
    return m_font.bold();
}

void Format::setBold(bool bold)
{
    m_font.setBold(bold);
}

bool Format::italic() const
{
    return m_font.italic();
}

void Format::setItalic(bool italic)
{
    m_font.setItalic(italic);
}

bool Format::underline() const
{
    return m_font.underline();
}

void Format::setUnderline(bool underline)
{
    m_font.setUnderline(underline);
}

bool Format::operator==(const Format &other) const
{
    return m_font == other.m_font && m_color == other.m_color;
}

bool Format::operator!=(const Format &other) const
{
    return !(*this == other);
}
