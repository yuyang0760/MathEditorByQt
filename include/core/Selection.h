#ifndef SELECTION_H
#define SELECTION_H

class Selection
{
public:
    struct Position
    {
        int paragraph;
        int position;
        
        bool operator==(const Position &other) const {
            return paragraph == other.paragraph && position == other.position;
        }
        
        bool operator<(const Position &other) const {
            if (paragraph < other.paragraph) return true;
            if (paragraph > other.paragraph) return false;
            return position < other.position;
        }
    };
    
    Selection();
    Selection(const Position &start, const Position &end);
    
    // 获取和设置起始位置
    Position start() const;
    void setStart(const Position &position);
    
    // 获取和设置结束位置
    Position end() const;
    void setEnd(const Position &position);
    
    // 获取归一化的位置（确保start <= end）
    Position normalizedStart() const;
    Position normalizedEnd() const;
    
    // 检查是否为空选择
    bool isEmpty() const;
    
    // 扩展选择
    void extend(const Position &position);
    
    // 收缩选择
    void collapseToStart();
    void collapseToEnd();
    
    // 比较操作
    bool operator==(const Selection &other) const;
    bool operator!=(const Selection &other) const;
    
private:
    Position m_start;
    Position m_end;
};

#endif // SELECTION_H
