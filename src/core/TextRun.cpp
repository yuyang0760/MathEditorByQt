#include "core/TextRun.h"

TextRun::TextRun() {}
TextRun::TextRun(const QString &text, const Format &format) : m_text(text), m_format(format) {}

QString TextRun::text() const { return m_text; }
void TextRun::setText(const QString &text) { m_text = text; }
Format TextRun::format() const { return m_format; }
void TextRun::setFormat(const Format &format) { m_format = format; }

void TextRun::insert(int position, const QString &text) { m_text.insert(position, text); }
void TextRun::remove(int position, int length) { m_text.remove(position, length); }
int TextRun::length() const { return m_text.length(); }