// ============================================================================
// SelectionController.cpp
// 选择控制器类的实现文件
// 负责处理文档的选择操作，如设置选择、扩展选择、移动选择等
// ============================================================================

#include "controller/SelectionController.h"

/**
 * @brief 构造函数
 * @param parent 父对象
 */
SelectionController::SelectionController(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief 获取当前选择
 * @return 当前选择
 */
Selection SelectionController::selection() const
{
    return m_selection;
}

/**
 * @brief 设置选择
 * @param selection 要设置的选择
 */
void SelectionController::setSelection(const Selection &selection)
{
    if (m_selection != selection)
    {
        m_selection = selection;
        emit selectionChanged(m_selection);
    }
}

/**
 * @brief 选择所有文本
 */
void SelectionController::selectAll()
{
    // 实现全选功能
    // 这里可以添加具体的全选逻辑
}

/**
 * @brief 选择指定位置的单词
 * @param position 单词位置
 */
void SelectionController::selectWord(const Selection::Position &position)
{
    // 实现选择单词功能
    // 这里可以添加具体的选择单词逻辑
}

/**
 * @brief 选择指定段落
 * @param paragraphIndex 段落索引
 */
void SelectionController::selectParagraph(int paragraphIndex)
{
    // 实现选择段落功能
    // 这里可以添加具体的选择段落逻辑
}

/**
 * @brief 扩展选择到指定位置
 * @param position 扩展到的位置
 */
void SelectionController::extendSelection(const Selection::Position &position)
{
    Selection newSelection = m_selection;
    newSelection.extend(position);
    setSelection(newSelection);
}

/**
 * @brief 移动选择
 * @param direction 移动方向
 * @param extend 是否扩展选择
 */
void SelectionController::moveSelection(int direction, bool extend)
{
    // 实现移动选择功能
    // 这里可以添加具体的移动选择逻辑
}

/**
 * @brief 检查是否有选择
 * @return 是否有选择
 */
bool SelectionController::hasSelection() const
{
    return !m_selection.isEmpty();
}

/**
 * @brief 检查选择是否为空
 * @return 选择是否为空
 */
bool SelectionController::isSelectionEmpty() const
{
    return m_selection.isEmpty();
}
