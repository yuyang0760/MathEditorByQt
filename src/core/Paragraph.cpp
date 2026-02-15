// ============================================================================
// Paragraph.cpp
// 段落类实现文件
// ============================================================================

#include "core/Paragraph.h"
#include "core/TextRun.h"
#include "core/MathObject.h"
#include "core/StyleManager.h"

Paragraph::Paragraph() {}

void Paragraph::appendText(const QString &text, const Format &format) {
    Item item;
    item.type = TextRunItem;
    item.data = QVariant::fromValue(TextRun(text, format));
    m_items.append(item);
}

void Paragraph::appendText(const QString &text, const QString &styleId, const Format &directFormat) {
    Item item;
    item.type = TextRunItem;
    item.data = QVariant::fromValue(TextRun(text, styleId, directFormat));
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

void Paragraph::insertItem(int index, const Item &item) {
    m_items.insert(index, item);
}

void Paragraph::replaceItems(int index, int count, const QList<Item> &newItems) {
    for (int i = 0; i < count; ++i) {
        m_items.removeAt(index);
    }
    for (int i = 0; i < newItems.size(); ++i) {
        m_items.insert(index + i, newItems[i]);
    }
}

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

QString Paragraph::styleId() const { return m_styleId; }
void Paragraph::setStyleId(const QString &styleId) { m_styleId = styleId; }

Format Paragraph::effectiveFormat(StyleManager *styleMgr) const {
    if (m_styleId.isEmpty() || !styleMgr) return Format();
    return styleMgr->resolveStyle(m_styleId);
}