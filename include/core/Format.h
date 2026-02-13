#ifndef FORMAT_H
#define FORMAT_H

#include <QFont>
#include <QColor>

class Format {
public:
    Format();
    Format(const QFont &font, const QColor &color = Qt::black);

    QFont font() const;
    void setFont(const QFont &font);
    QColor color() const;
    void setColor(const QColor &color);

    bool bold() const;
    void setBold(bool bold);
    bool italic() const;
    void setItalic(bool italic);
    bool underline() const;
    void setUnderline(bool underline);

    bool operator==(const Format &other) const;
    bool operator!=(const Format &other) const;

private:
    QFont m_font;
    QColor m_color;
};

#endif // FORMAT_H