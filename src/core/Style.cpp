// ============================================================================
// Style.cpp
// 样式类实现文件
// ============================================================================

#include "core/Style.h"

Style::Style(const QString &id, const QString &name, const Format &format, const QString &parentId)
    : m_id(id), m_name(name), m_format(format), m_parentId(parentId) {}

QString Style::id() const { return m_id; }
void Style::setId(const QString &id) { m_id = id; }

QString Style::name() const { return m_name; }
void Style::setName(const QString &name) { m_name = name; }

Format Style::format() const { return m_format; }
void Style::setFormat(const Format &format) { m_format = format; }

QString Style::parentId() const { return m_parentId; }
void Style::setParentId(const QString &parentId) { m_parentId = parentId; }

bool Style::operator==(const Style &other) const {
    return m_id == other.m_id &&
           m_name == other.m_name &&
           m_format == other.m_format &&
           m_parentId == other.m_parentId;
}

bool Style::operator!=(const Style &other) const {
    return !(*this == other);
}