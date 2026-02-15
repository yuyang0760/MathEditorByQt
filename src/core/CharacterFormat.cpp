// ============================================================================
// CharacterFormat.cpp
// 字符格式类实现文件
// 实现字符格式的各种操作
// ============================================================================

#include "core/CharacterFormat.h"

CharacterFormat::CharacterFormat()
    : m_fontFamily("")
    , m_hasFontFamily(false)
    , m_fontSize(0)
    , m_hasFontSize(false)
    , m_color(QColor())
    , m_hasColor(false)
    , m_boldState(PropertyState::Unset)
    , m_italicState(PropertyState::Unset)
    , m_underlineState(PropertyState::Unset)
{
}

CharacterFormat::CharacterFormat(const QFont &font, const QColor &color)
    : m_fontFamily(font.family())
    , m_hasFontFamily(!font.family().isEmpty())
    , m_fontSize(font.pointSize())
    , m_hasFontSize(font.pointSize() > 0)
    , m_color(color)
    , m_hasColor(color.isValid())
    , m_boldState(font.bold() ? PropertyState::SetTrue : PropertyState::SetFalse)
    , m_italicState(font.italic() ? PropertyState::SetTrue : PropertyState::SetFalse)
    , m_underlineState(font.underline() ? PropertyState::SetTrue : PropertyState::SetFalse)
{
}

QString CharacterFormat::fontFamily() const {
    return m_fontFamily;
}

void CharacterFormat::setFontFamily(const QString &family) {
    m_fontFamily = family;
    m_hasFontFamily = true;
}

bool CharacterFormat::hasFontFamily() const {
    return m_hasFontFamily;
}

void CharacterFormat::clearFontFamily() {
    m_fontFamily = "";
    m_hasFontFamily = false;
}

int CharacterFormat::fontSize() const {
    return m_fontSize;
}

void CharacterFormat::setFontSize(int size) {
    m_fontSize = size;
    m_hasFontSize = true;
}

bool CharacterFormat::hasFontSize() const {
    return m_hasFontSize;
}

void CharacterFormat::clearFontSize() {
    m_fontSize = 0;
    m_hasFontSize = false;
}

QColor CharacterFormat::color() const {
    return m_color;
}

void CharacterFormat::setColor(const QColor &color) {
    m_color = color;
    m_hasColor = true;
}

bool CharacterFormat::hasColor() const {
    return m_hasColor;
}

void CharacterFormat::clearColor() {
    m_color = QColor();
    m_hasColor = false;
}

PropertyState CharacterFormat::boldState() const {
    return m_boldState;
}

void CharacterFormat::setBold(bool bold) {
    m_boldState = bold ? PropertyState::SetTrue : PropertyState::SetFalse;
}

bool CharacterFormat::hasBold() const {
    return m_boldState != PropertyState::Unset;
}

void CharacterFormat::clearBold() {
    m_boldState = PropertyState::Unset;
}

PropertyState CharacterFormat::italicState() const {
    return m_italicState;
}

void CharacterFormat::setItalic(bool italic) {
    m_italicState = italic ? PropertyState::SetTrue : PropertyState::SetFalse;
}

bool CharacterFormat::hasItalic() const {
    return m_italicState != PropertyState::Unset;
}

void CharacterFormat::clearItalic() {
    m_italicState = PropertyState::Unset;
}

PropertyState CharacterFormat::underlineState() const {
    return m_underlineState;
}

void CharacterFormat::setUnderline(bool underline) {
    m_underlineState = underline ? PropertyState::SetTrue : PropertyState::SetFalse;
}

bool CharacterFormat::hasUnderline() const {
    return m_underlineState != PropertyState::Unset;
}

void CharacterFormat::clearUnderline() {
    m_underlineState = PropertyState::Unset;
}

CharacterFormat CharacterFormat::merge(const CharacterFormat &base, const CharacterFormat &override) {
    CharacterFormat result = base;
    
    if (override.hasFontFamily()) {
        result.setFontFamily(override.fontFamily());
    }
    if (override.hasFontSize()) {
        result.setFontSize(override.fontSize());
    }
    if (override.hasColor()) {
        result.setColor(override.color());
    }
    if (override.hasBold()) {
        result.m_boldState = override.m_boldState;
    }
    if (override.hasItalic()) {
        result.m_italicState = override.m_italicState;
    }
    if (override.hasUnderline()) {
        result.m_underlineState = override.m_underlineState;
    }
    
    return result;
}

QFont CharacterFormat::toFont(const QFont &defaultFont) const {
    QFont font = defaultFont;
    
    if (hasFontFamily()) {
        font.setFamily(m_fontFamily);
    }
    if (hasFontSize()) {
        font.setPointSize(m_fontSize);
    }
    if (hasBold()) {
        font.setBold(m_boldState == PropertyState::SetTrue);
    }
    if (hasItalic()) {
        font.setItalic(m_italicState == PropertyState::SetTrue);
    }
    if (hasUnderline()) {
        font.setUnderline(m_underlineState == PropertyState::SetTrue);
    }
    
    return font;
}

bool CharacterFormat::operator==(const CharacterFormat &other) const {
    return m_fontFamily == other.m_fontFamily
        && m_hasFontFamily == other.m_hasFontFamily
        && m_fontSize == other.m_fontSize
        && m_hasFontSize == other.m_hasFontSize
        && m_color == other.m_color
        && m_hasColor == other.m_hasColor
        && m_boldState == other.m_boldState
        && m_italicState == other.m_italicState
        && m_underlineState == other.m_underlineState;
}

bool CharacterFormat::operator!=(const CharacterFormat &other) const {
    return !(*this == other);
}
