#include "core/Selection.h"

Selection::Selection()
{
}

Selection::Selection(const Position &start, const Position &end)
    : m_start(start), m_end(end)
{
}

Selection::Position Selection::start() const
{
    return m_start;
}

void Selection::setStart(const Position &position)
{
    m_start = position;
}

Selection::Position Selection::end() const
{
    return m_end;
}

void Selection::setEnd(const Position &position)
{
    m_end = position;
}

Selection::Position Selection::normalizedStart() const
{
    if (m_start < m_end)
        return m_start;
    return m_end;
}

Selection::Position Selection::normalizedEnd() const
{
    if (m_start < m_end)
        return m_end;
    return m_start;
}

bool Selection::isEmpty() const
{
    return m_start == m_end;
}

void Selection::extend(const Position &position)
{
    m_end = position;
}

void Selection::collapseToStart()
{
    m_end = m_start;
}

void Selection::collapseToEnd()
{
    m_start = m_end;
}

bool Selection::operator==(const Selection &other) const
{
    return m_start == other.m_start && m_end == other.m_end;
}

bool Selection::operator!=(const Selection &other) const
{
    return !(*this == other);
}
