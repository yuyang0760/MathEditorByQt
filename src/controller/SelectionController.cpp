// ============================================================================
// SelectionController.cpp
// 选择控制器实现文件
// 管理文档中的选择区域和光标位置，处理选择变化通知
// ============================================================================

#include "controller/SelectionController.h"

/**
 * @brief 构造函数
 * 初始化选择控制器，设置父对象和默认选择
 * @param parent 父对象指针
 */
SelectionController::SelectionController(QObject *parent)
    : QObject(parent), m_selection() {}

/**
 * @brief 设置选择区域
 * 
 * 该方法设置新的选择区域，如果选择发生变化，会发出selectionChanged信号
 * 
 * @param selection 新的选择对象
 */
void SelectionController::setSelection(const Selection &selection) {
    if (m_selection != selection) {
        m_selection = selection;
        emit selectionChanged(m_selection);
    }
}

/**
 * @brief 获取当前选择区域
 * @return 当前选择对象
 */
Selection SelectionController::selection() const { return m_selection; }

/**
 * @brief 检查是否有选择区域
 * @return 如果有选择区域返回true，否则返回false
 */
bool SelectionController::hasSelection() const { return !m_selection.isEmpty(); }