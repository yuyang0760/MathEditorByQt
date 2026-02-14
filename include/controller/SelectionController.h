// ============================================================================
// SelectionController.h
// 选择控制器头文件
// 管理文档中的选择区域和光标位置
// ============================================================================

#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include "core/Selection.h"
#include <QObject>

/**
 * @brief 选择控制器类
 * 
 * SelectionController类负责管理文档中的选择区域和光标位置。
 * 它提供选择操作的接口，并在选择发生变化时发出信号通知其他组件。
 * 
 * 主要功能：
 * - 选择区域管理
 * - 光标位置控制
 * - 选择状态检查
 * - 选择变化通知
 */
class SelectionController : public QObject {
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象指针
     */
    SelectionController(QObject *parent = nullptr);

    /**
     * @brief 设置选择区域
     * @param selection 新的选择对象
     */
    void setSelection(const Selection &selection);
    
    /**
     * @brief 获取当前选择区域
     * @return 当前选择对象
     */
    Selection selection() const;
    
    /**
     * @brief 检查是否有选择区域
     * @return 如果有选择区域返回true
     */
    bool hasSelection() const;

signals:
    /**
     * @brief 选择变化信号
     * @param selection 新的选择对象
     */
    void selectionChanged(const Selection &selection);

private:
    Selection m_selection; ///< 当前选择区域
};

#endif // SELECTIONCONTROLLER_H