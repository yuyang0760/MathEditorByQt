// ============================================================================
// SelectionController.h
// 选择控制器类的头文件
// 负责处理文档的选择操作，如设置选择、扩展选择、移动选择等
// ============================================================================

#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include "core/Selection.h"
#include <QObject>

/**
 * @class SelectionController
 * @brief 选择控制器类
 * 
 * 负责处理文档的选择操作，如设置选择、扩展选择、移动选择等。
 * 管理文档中的文本选择状态，并在选择发生变化时发出信号。
 */
class SelectionController : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父对象
     */
    SelectionController(QObject *parent = nullptr);
    
    /**
     * @brief 获取当前选择
     * @return 当前选择
     */
    Selection selection() const;
    
    /**
     * @brief 设置选择
     * @param selection 要设置的选择
     */
    void setSelection(const Selection &selection);
    
    /**
     * @brief 选择所有文本
     */
    void selectAll();
    
    /**
     * @brief 选择指定位置的单词
     * @param position 单词位置
     */
    void selectWord(const Selection::Position &position);
    
    /**
     * @brief 选择指定段落
     * @param paragraphIndex 段落索引
     */
    void selectParagraph(int paragraphIndex);
    
    /**
     * @brief 扩展选择到指定位置
     * @param position 扩展到的位置
     */
    void extendSelection(const Selection::Position &position);
    
    /**
     * @brief 移动选择
     * @param direction 移动方向
     * @param extend 是否扩展选择
     */
    void moveSelection(int direction, bool extend = false);
    
    /**
     * @brief 检查是否有选择
     * @return 是否有选择
     */
    bool hasSelection() const;
    
    /**
     * @brief 检查选择是否为空
     * @return 选择是否为空
     */
    bool isSelectionEmpty() const;
    
signals:
    /**
     * @brief 选择变化信号
     * 当选择发生变化时发出此信号
     * @param selection 新的选择
     */
    void selectionChanged(const Selection &selection);
    
private:
    /**
     * @brief 当前选择
     */
    Selection m_selection;
};

#endif // SELECTIONCONTROLLER_H
