#include "core/Paragraph.h"
#include "core/TextRun.h"
#include "core/MathObject.h"

Paragraph::Paragraph() {}

void Paragraph::appendText(const QString &text, const Format &format) {
    Item item;
    item.type = TextRunItem;
    item.data = QVariant::fromValue(TextRun(text, format));
    m_items.append(item);
}

void Paragraph::appendMathObject(const MathObject &obj) {
    Item item;
    item.type = MathObjectItem;
    item.data = QVariant::fromValue(obj);
    m_items.append(item);
}

int Paragraph::itemCount() const { return m_items.size(); }
const Paragraph::Item &Paragraph::itemAt(int index) const { return m_items[index]; }
Paragraph::Item &Paragraph::itemAt(int index) { return m_items[index]; }
void Paragraph::insertItem(int index, const Item &item) { m_items.insert(index, item); }

QString Paragraph::plainText() const {
    QString result;
    for (const auto &item : m_items) {
        if (item.type == TextRunItem) {
            result += item.data.value<TextRun>().text();
        } else {
            result += "[Math]";
        }
    }
    return result;
}