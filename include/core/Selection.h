// ============================================================================
// Selection.h
// 选择和位置类头文件
// 定义文档中的位置和选择区域
// ============================================================================

#ifndef SELECTION_H
#define SELECTION_H

/**
 * @brief 位置结构体
 * 
 * Position结构体表示文档中的一个精确位置，使用三级索引：
 * - 段落索引：文档中的段落编号
 * - 项索引：段落中的内容项编号
 * - 偏移量：项内的字符位置或公式前后位置
 */
struct Position {
    int paragraph;      ///< 段落索引（从0开始）
    int item;           ///< 内容项索引（在段落中，从0开始）
    int offset;         ///< 项内偏移：对于文本是字符位置（0-长度），对于公式是0（前）或1（后）
};

inline bool operator==(const Position &a, const Position &b) {
    return a.paragraph == b.paragraph && a.item == b.item && a.offset == b.offset;
}
inline bool operator<(const Position &a, const Position &b) {
    if (a.paragraph != b.paragraph) return a.paragraph < b.paragraph;
    if (a.item != b.item) return a.item < b.item;
    return a.offset < b.offset;
}
inline bool operator<=(const Position &a, const Position &b) {
    return a == b || a < b;
}
inline bool operator>=(const Position &a, const Position &b) {
    return !(a < b);
}

/**
 * @brief 选择类
 * 
 * Selection类表示文档中的一个选择区域，由起始位置和结束位置定义。
 * 支持选择操作的各种功能，包括规范化、扩展、折叠等。
 * 
 * 主要功能：
 * - 选择区域管理
 * - 位置规范化
 * - 选择操作（扩展、折叠）
 * - 选择状态检查
 */
class Selection {
public:
    /**
     * @brief 默认构造函数
     * 创建空的选择区域
     */
    Selection();
    
    /**
     * @brief 带参数构造函数
     * @param start 选择起始位置
     * @param end 选择结束位置
     */
    Selection(const Position &start, const Position &end);

    /**
     * @brief 获取起始位置
     * @return 选择起始位置
     */
    Position start() const;
    
    /**
     * @brief 设置起始位置
     * @param pos 新的起始位置
     */
    void setStart(const Position &pos);
    
    /**
     * @brief 获取结束位置
     * @return 选择结束位置
     */
    Position end() const;
    
    /**
     * @brief 设置结束位置
     * @param pos 新的结束位置
     */
    void setEnd(const Position &pos);

    /**
     * @brief 获取规范化起始位置
     * @return 规范化后的起始位置（保证start <= end）
     */
    Position normalizedStart() const;
    
    /**
     * @brief 获取规范化结束位置
     * @return 规范化后的结束位置（保证start <= end）
     */
    Position normalizedEnd() const;

    /**
     * @brief 检查选择是否为空
     * @return 如果选择为空（起始位置等于结束位置）返回true
     */
    bool isEmpty() const;
    
    /**
     * @brief 扩展选择到指定位置
     * @param pos 要扩展到的位置
     */
    void extend(const Position &pos);
    
    /**
     * @brief 折叠选择到起始位置
     * 将结束位置设置为起始位置，使选择为空
     */
    void collapseToStart();
    
    /**
     * @brief 折叠选择到结束位置
     * 将起始位置设置为结束位置，使选择为空
     */
    void collapseToEnd();

    /**
     * @brief 等于操作符重载
     * @param other 要比较的另一个选择对象
     * @return 如果两个选择相同返回true
     */
    bool operator==(const Selection &other) const;
    
    /**
     * @brief 不等于操作符重载
     * @param other 要比较的另一个选择对象
     * @return 如果两个选择不同返回true
     */
    bool operator!=(const Selection &other) const;
    
    /**
     * @brief 检查选择是否有效
     * @return 如果选择有效返回true
     */
    bool isValid() const;
    
    /**
     * @brief 检查位置是否在选择区域内
     * @param pos 要检查的位置
     * @return 如果位置在选择区域内返回true
     */
    bool contains(const Position &pos) const;

private:
    Position m_start;  ///< 选择起始位置
    Position m_end;    ///< 选择结束位置
};

#endif // SELECTION_H