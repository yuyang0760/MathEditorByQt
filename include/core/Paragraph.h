#ifndef PARAGRAPH_H
#define PARAGRAPH_H

#include <QVariant> // 用于存储 TextRun 或 MathObject
#include <QList>

class Format;
class MathObject;
class TextRun;

class Paragraph {
public:
    enum ItemType { TextRunItem, MathObjectItem };

    struct Item {
        ItemType type;
        QVariant data; // TextRun 或 MathObject
    };

    Paragraph();

    void appendText(const QString &text, const Format &format);
    void appendMathObject(const MathObject &obj);

    int itemCount() const;
    const Item &itemAt(int index) const;
    Item &itemAt(int index);
    void insertItem(int index, const Item &item);

    QString plainText() const; // 用于调试

private:
    QList<Item> m_items;
};

#endif // PARAGRAPH_H