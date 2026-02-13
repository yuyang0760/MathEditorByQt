#ifndef SELECTION_H
#define SELECTION_H

struct Position {
    int paragraph;      // 段落索引
    int item;           // 内容项索引（在段落中）
    int offset;         // 项内偏移：对于文本是字符位置，对于公式是0
};

inline bool operator==(const Position &a, const Position &b) {
    return a.paragraph == b.paragraph && a.item == b.item && a.offset == b.offset;
}
inline bool operator<(const Position &a, const Position &b) {
    if (a.paragraph != b.paragraph) return a.paragraph < b.paragraph;
    if (a.item != b.item) return a.item < b.item;
    return a.offset < b.offset;
}

class Selection {
public:
    Selection();
    Selection(const Position &start, const Position &end);

    Position start() const;
    void setStart(const Position &pos);
    Position end() const;
    void setEnd(const Position &pos);

    Position normalizedStart() const;
    Position normalizedEnd() const;

    bool isEmpty() const;
    void extend(const Position &pos);
    void collapseToStart();
    void collapseToEnd();

    bool operator==(const Selection &other) const;
    bool operator!=(const Selection &other) const;

private:
    Position m_start, m_end;
};

#endif // SELECTION_H