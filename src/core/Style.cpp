// ============================================================================
// Style.cpp
// 样式类实现文件
// 实现样式类的各种操作
// ============================================================================

#include "core/Style.h"

Style::Style()
    : m_id(QString()),
      m_name(QString()),
      m_type(StyleType::Paragraph),
      m_category(StyleCategory::Custom),
      m_description(QString()),
      m_isBuiltIn(false),
      m_isHidden(false),
      m_priority(0),
      m_nextStyleId(QString()),
      m_charFormat(CharacterFormat()),
      m_paraFormat(ParagraphFormat()),
      m_parentId(QString()) {}

Style::Style(const QString &id, const QString &name, StyleType type, 
             const CharacterFormat &charFormat, const ParagraphFormat &paraFormat, 
             const QString &parentId)
    : m_id(id),
      m_name(name),
      m_type(type),
      m_category(StyleCategory::Custom),
      m_description(QString()),
      m_isBuiltIn(false),
      m_isHidden(false),
      m_priority(0),
      m_nextStyleId(QString()),
      m_charFormat(charFormat),
      m_paraFormat(paraFormat),
      m_parentId(parentId) {}

QString Style::id() const { 
    return m_id; 
}

void Style::setId(const QString &id) { 
    m_id = id; 
}

QString Style::name() const { 
    return m_name; 
}

void Style::setName(const QString &name) { 
    m_name = name; 
}

StyleType Style::type() const { 
    return m_type; 
}

void Style::setType(StyleType type) { 
    m_type = type; 
}

StyleCategory Style::category() const { 
    return m_category; 
}

void Style::setCategory(StyleCategory category) { 
    m_category = category; 
}

QString Style::description() const { 
    return m_description; 
}

void Style::setDescription(const QString &description) { 
    m_description = description; 
}

bool Style::isBuiltIn() const { 
    return m_isBuiltIn; 
}

void Style::setBuiltIn(bool builtIn) { 
    m_isBuiltIn = builtIn; 
}

bool Style::isHidden() const { 
    return m_isHidden; 
}

void Style::setHidden(bool hidden) { 
    m_isHidden = hidden; 
}

int Style::priority() const { 
    return m_priority; 
}

void Style::setPriority(int priority) { 
    m_priority = priority; 
}

QString Style::nextStyleId() const { 
    return m_nextStyleId; 
}

void Style::setNextStyleId(const QString &nextStyleId) { 
    m_nextStyleId = nextStyleId; 
}

CharacterFormat Style::characterFormat() const { 
    return m_charFormat; 
}

void Style::setCharacterFormat(const CharacterFormat &format) { 
    m_charFormat = format; 
}

ParagraphFormat Style::paragraphFormat() const { 
    return m_paraFormat; 
}

void Style::setParagraphFormat(const ParagraphFormat &format) { 
    m_paraFormat = format; 
}

QString Style::parentId() const { 
    return m_parentId; 
}

void Style::setParentId(const QString &parentId) { 
    m_parentId = parentId; 
}

bool Style::hasParent() const { 
    return !m_parentId.isEmpty(); 
}

bool Style::canDelete() const { 
    return !m_isBuiltIn; 
}

bool Style::canModify() const { 
    return true; 
}

Style Style::clone() const {
    Style newStyle;
    newStyle.m_id = m_id;
    newStyle.m_name = m_name;
    newStyle.m_type = m_type;
    newStyle.m_category = m_category;
    newStyle.m_description = m_description;
    newStyle.m_isBuiltIn = m_isBuiltIn;
    newStyle.m_isHidden = m_isHidden;
    newStyle.m_priority = m_priority;
    newStyle.m_nextStyleId = m_nextStyleId;
    newStyle.m_charFormat = m_charFormat;
    newStyle.m_paraFormat = m_paraFormat;
    newStyle.m_parentId = m_parentId;
    return newStyle;
}

bool Style::operator==(const Style &other) const {
    return m_id == other.m_id &&
           m_name == other.m_name &&
           m_type == other.m_type &&
           m_category == other.m_category &&
           m_description == other.m_description &&
           m_isBuiltIn == other.m_isBuiltIn &&
           m_isHidden == other.m_isHidden &&
           m_priority == other.m_priority &&
           m_nextStyleId == other.m_nextStyleId &&
           m_charFormat == other.m_charFormat &&
           m_paraFormat == other.m_paraFormat &&
           m_parentId == other.m_parentId;
}

bool Style::operator!=(const Style &other) const {
    return !(*this == other);
}
