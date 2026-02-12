#include "controller/SelectionController.h"

SelectionController::SelectionController(QObject *parent)
    : QObject(parent)
{
}

Selection SelectionController::selection() const
{
    return m_selection;
}

void SelectionController::setSelection(const Selection &selection)
{
    if (m_selection != selection)
    {
        m_selection = selection;
        emit selectionChanged(m_selection);
    }
}

void SelectionController::selectAll()
{
    // 实现全选功能
    // 这里可以添加具体的全选逻辑
}

void SelectionController::selectWord(const Selection::Position &position)
{
    // 实现选择单词功能
    // 这里可以添加具体的选择单词逻辑
}

void SelectionController::selectParagraph(int paragraphIndex)
{
    // 实现选择段落功能
    // 这里可以添加具体的选择段落逻辑
}

void SelectionController::extendSelection(const Selection::Position &position)
{
    Selection newSelection = m_selection;
    newSelection.extend(position);
    setSelection(newSelection);
}

void SelectionController::moveSelection(int direction, bool extend)
{
    // 实现移动选择功能
    // 这里可以添加具体的移动选择逻辑
}

bool SelectionController::hasSelection() const
{
    return !m_selection.isEmpty();
}

bool SelectionController::isSelectionEmpty() const
{
    return m_selection.isEmpty();
}
