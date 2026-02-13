#include "core/Selection.h"

Selection::Selection() : m_start{0,0,0}, m_end{0,0,0} {}
Selection::Selection(const Position &start, const Position &end) : m_start(start), m_end(end) {}

Position Selection::start() const { return m_start; }
void Selection::setStart(const Position &pos) { m_start = pos; }
Position Selection::end() const { return m_end; }
void Selection::setEnd(const Position &pos) { m_end = pos; }

Position Selection::normalizedStart() const { return m_start < m_end ? m_start : m_end; }
Position Selection::normalizedEnd() const { return m_start < m_end ? m_end : m_start; }

bool Selection::isEmpty() const { return m_start == m_end; }
void Selection::extend(const Position &pos) { m_end = pos; }
void Selection::collapseToStart() { m_end = m_start; }
void Selection::collapseToEnd() { m_start = m_end; }

bool Selection::operator==(const Selection &other) const { return m_start == other.m_start && m_end == other.m_end; }
bool Selection::operator!=(const Selection &other) const { return !(*this == other); }