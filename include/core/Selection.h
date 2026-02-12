// ============================================================================
// Selection.h
// 选择类的头文件
// 表示文档中的文本选择范围
// ============================================================================

#ifndef SELECTION_H
#define SELECTION_H

/**
 * @class Selection
 * @brief 选择类
 * 
 * 表示文档中的文本选择范围。
 * 包含起始位置和结束位置，提供选择的各种操作。
 */
class Selection
{
public:
    /**
     * @struct Position
     * @brief 位置结构体
     * 
     * 表示文档中的位置，包含段落索引和段落内的位置。
     */
    struct Position
    {
        /**
         * @brief 段落索引
         */
        int paragraph;
        
        /**
         * @brief 段落内的位置
         */
        int position;
        
        /**
         * @brief 比较两个位置是否相等
         * @param other 另一个位置
         * @return 是否相等
         */
        bool operator==(const Position &other) const {
            return paragraph == other.paragraph && position == other.position;
        }
        
        /**
         * @brief 比较两个位置的大小
         * @param other 另一个位置
         * @return 当前位置是否小于另一个位置
         */
        bool operator<(const Position &other) const {
            if (paragraph < other.paragraph) return true;
            if (paragraph > other.paragraph) return false;
            return position < other.position;
        }
    };
    
    /**
     * @brief 构造函数
     * 创建一个空选择
     */
    Selection();
    
    /**
     * @brief 构造函数
     * @param start 起始位置
     * @param end 结束位置
     */
    Selection(const Position &start, const Position &end);
    
    /**
     * @brief 获取起始位置
     * @return 起始位置
     */
    Position start() const;
    
    /**
     * @brief 设置起始位置
     * @param position 起始位置
     */
    void setStart(const Position &position);
    
    /**
     * @brief 获取结束位置
     * @return 结束位置
     */
    Position end() const;
    
    /**
     * @brief 设置结束位置
     * @param position 结束位置
     */
    void setEnd(const Position &position);
    
    /**
     * @brief 获取归一化的起始位置（确保start <= end）
     * @return 归一化的起始位置
     */
    Position normalizedStart() const;
    
    /**
     * @brief 获取归一化的结束位置（确保start <= end）
     * @return 归一化的结束位置
     */
    Position normalizedEnd() const;
    
    /**
     * @brief 检查是否为空选择
     * @return 是否为空选择
     */
    bool isEmpty() const;
    
    /**
     * @brief 扩展选择到指定位置
     * @param position 扩展到的位置
     */
    void extend(const Position &position);
    
    /**
     * @brief 收缩选择到起始位置
     */
    void collapseToStart();
    
    /**
     * @brief 收缩选择到结束位置
     */
    void collapseToEnd();
    
    /**
     * @brief 比较两个选择是否相等
     * @param other 另一个选择
     * @return 是否相等
     */
    bool operator==(const Selection &other) const;
    
    /**
     * @brief 比较两个选择是否不相等
     * @param other 另一个选择
     * @return 是否不相等
     */
    bool operator!=(const Selection &other) const;
    
private:
    /**
     * @brief 起始位置
     */
    Position m_start;
    
    /**
     * @brief 结束位置
     */
    Position m_end;
};

#endif // SELECTION_H
