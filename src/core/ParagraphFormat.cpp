// ============================================================================
// ParagraphFormat.cpp
// 段落格式类实现文件
// 实现段落格式的各种操作
// ============================================================================

#include "core/ParagraphFormat.h"

ParagraphFormat::ParagraphFormat()
    : m_alignment(Alignment::Left)
    , m_hasAlignment(false)
    , m_lineSpacingType(LineSpacingType::Single)
    , m_hasLineSpacingType(false)
    , m_lineSpacing(0.0)
    , m_hasLineSpacing(false)
    , m_lineHeight(0.0)
    , m_hasLineHeight(false)
    , m_spaceBefore(0)
    , m_hasSpaceBefore(false)
    , m_spaceAfter(0)
    , m_hasSpaceAfter(false)
    , m_firstLineIndent(0)
    , m_hasFirstLineIndent(false)
{
}

Alignment ParagraphFormat::alignment() const {
    return m_alignment;
}

void ParagraphFormat::setAlignment(Alignment align) {
    m_alignment = align;
    m_hasAlignment = true;
}

bool ParagraphFormat::hasAlignment() const {
    return m_hasAlignment;
}

void ParagraphFormat::clearAlignment() {
    m_alignment = Alignment::Left;
    m_hasAlignment = false;
}

qreal ParagraphFormat::lineSpacing() const {
    return m_lineSpacing;
}

void ParagraphFormat::setLineSpacing(qreal spacing) {
    m_lineSpacing = spacing;
    m_hasLineSpacing = true;
}

bool ParagraphFormat::hasLineSpacing() const {
    return m_hasLineSpacing;
}

void ParagraphFormat::clearLineSpacing() {
    m_lineSpacing = 0.0;
    m_hasLineSpacing = false;
}

LineSpacingType ParagraphFormat::lineSpacingType() const {
    return m_lineSpacingType;
}

void ParagraphFormat::setLineSpacingType(LineSpacingType type) {
    m_lineSpacingType = type;
    m_hasLineSpacingType = true;
}

bool ParagraphFormat::hasLineSpacingType() const {
    return m_hasLineSpacingType;
}

void ParagraphFormat::clearLineSpacingType() {
    m_lineSpacingType = LineSpacingType::Single;
    m_hasLineSpacingType = false;
}

qreal ParagraphFormat::lineHeight() const {
    return m_lineHeight;
}

void ParagraphFormat::setLineHeight(qreal height) {
    m_lineHeight = height;
    m_hasLineHeight = true;
}

bool ParagraphFormat::hasLineHeight() const {
    return m_hasLineHeight;
}

void ParagraphFormat::clearLineHeight() {
    m_lineHeight = 0.0;
    m_hasLineHeight = false;
}

int ParagraphFormat::spaceBefore() const {
    return m_spaceBefore;
}

void ParagraphFormat::setSpaceBefore(int space) {
    m_spaceBefore = space;
    m_hasSpaceBefore = true;
}

bool ParagraphFormat::hasSpaceBefore() const {
    return m_hasSpaceBefore;
}

void ParagraphFormat::clearSpaceBefore() {
    m_spaceBefore = 0;
    m_hasSpaceBefore = false;
}

int ParagraphFormat::spaceAfter() const {
    return m_spaceAfter;
}

void ParagraphFormat::setSpaceAfter(int space) {
    m_spaceAfter = space;
    m_hasSpaceAfter = true;
}

bool ParagraphFormat::hasSpaceAfter() const {
    return m_hasSpaceAfter;
}

void ParagraphFormat::clearSpaceAfter() {
    m_spaceAfter = 0;
    m_hasSpaceAfter = false;
}

int ParagraphFormat::firstLineIndent() const {
    return m_firstLineIndent;
}

void ParagraphFormat::setFirstLineIndent(int indent) {
    m_firstLineIndent = indent;
    m_hasFirstLineIndent = true;
}

bool ParagraphFormat::hasFirstLineIndent() const {
    return m_hasFirstLineIndent;
}

void ParagraphFormat::clearFirstLineIndent() {
    m_firstLineIndent = 0;
    m_hasFirstLineIndent = false;
}

ParagraphFormat ParagraphFormat::merge(const ParagraphFormat &base, const ParagraphFormat &override) {
    ParagraphFormat result = base;
    
    if (override.hasAlignment()) {
        result.setAlignment(override.alignment());
    }
    if (override.hasLineSpacingType()) {
        result.setLineSpacingType(override.lineSpacingType());
    }
    if (override.hasLineSpacing()) {
        result.setLineSpacing(override.lineSpacing());
    }
    if (override.hasLineHeight()) {
        result.setLineHeight(override.lineHeight());
    }
    if (override.hasSpaceBefore()) {
        result.setSpaceBefore(override.spaceBefore());
    }
    if (override.hasSpaceAfter()) {
        result.setSpaceAfter(override.spaceAfter());
    }
    if (override.hasFirstLineIndent()) {
        result.setFirstLineIndent(override.firstLineIndent());
    }
    
    return result;
}

bool ParagraphFormat::operator==(const ParagraphFormat &other) const {
    return m_alignment == other.m_alignment
        && m_hasAlignment == other.m_hasAlignment
        && m_lineSpacingType == other.m_lineSpacingType
        && m_hasLineSpacingType == other.m_hasLineSpacingType
        && m_lineSpacing == other.m_lineSpacing
        && m_hasLineSpacing == other.m_hasLineSpacing
        && m_lineHeight == other.m_lineHeight
        && m_hasLineHeight == other.m_hasLineHeight
        && m_spaceBefore == other.m_spaceBefore
        && m_hasSpaceBefore == other.m_hasSpaceBefore
        && m_spaceAfter == other.m_spaceAfter
        && m_hasSpaceAfter == other.m_hasSpaceAfter
        && m_firstLineIndent == other.m_firstLineIndent
        && m_hasFirstLineIndent == other.m_hasFirstLineIndent;
}

bool ParagraphFormat::operator!=(const ParagraphFormat &other) const {
    return !(*this == other);
}
