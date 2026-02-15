// ============================================================================
// TextRun.cpp
// 文本片段类实现文件
// 
// TextRun 类表示文档中的一个文本片段，包含文本内容和格式信息。
// 支持样式继承和直接格式设置两种格式应用方式。
// ============================================================================

#include "core/TextRun.h"
#include "core/StyleManager.h"

TextRun::TextRun() : m_hasDirectFormat(false) {}

TextRun::TextRun(const QString &text, const CharacterFormat &charFormat)
    : m_text(text), m_directFormat(charFormat), m_hasDirectFormat(true) {}

TextRun::TextRun(const QString &text, const QString &styleId, const CharacterFormat &directCharFormat)
    : m_text(text), m_styleId(styleId), m_directFormat(directCharFormat), m_hasDirectFormat(true) {}

QString TextRun::text() const { 
    return m_text; 
}

void TextRun::setText(const QString &text) { 
    m_text = text; 
}

void TextRun::insert(int position, const QString &text) {
    m_text.insert(position, text);
}

void TextRun::remove(int position, int length) {
    m_text.remove(position, length);
}

int TextRun::length() const { 
    return m_text.length(); 
}

QString TextRun::styleId() const { 
    return m_styleId; 
}

void TextRun::setStyleId(const QString &styleId) { 
    m_styleId = styleId; 
}

bool TextRun::hasDirectFormat() const { 
    return m_hasDirectFormat; 
}

CharacterFormat TextRun::directFormat() const { 
    return m_directFormat; 
}

void TextRun::setDirectFormat(const CharacterFormat &format) {
    m_directFormat = format;
    m_hasDirectFormat = true;
}

void TextRun::clearDirectFormat() { 
    m_hasDirectFormat = false; 
}

CharacterFormat TextRun::effectiveFormat(StyleManager *styleMgr) const {
    CharacterFormat base;
    if (!m_styleId.isEmpty() && styleMgr) {
        base = styleMgr->resolveCharacterFormat(m_styleId);
    }
    if (m_hasDirectFormat) {
        return CharacterFormat::merge(base, m_directFormat);
    }
    return base;
}
