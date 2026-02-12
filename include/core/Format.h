#ifndef FORMAT_H
#define FORMAT_H

#include <QFont>
#include <QColor>

class Format
{
public:
    Format();
    Format(const QFont &font, const QColor &color = Qt::black);
    
    // 获取和设置字体
    QFont font() const;
    void setFont(const QFont &font);
    
    // 获取和设置颜色
    QColor color() const;
    void setColor(const QColor &color);
    
    // 字体样式操作
    bool bold() const;
    void setBold(bool bold);
    
    bool italic() const;
    void setItalic(bool italic);
    
    bool underline() const;
    void setUnderline(bool underline);
    
    // 比较操作
    bool operator==(const Format &other) const;
    bool operator!=(const Format &other) const;
    
private:
    QFont m_font;
    QColor m_color;
};

#endif // FORMAT_H
