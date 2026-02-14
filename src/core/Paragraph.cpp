// ============================================================================
// Paragraph.cpp
// 段落类实现文件
// 实现段落的基本功能，包括内容项管理和文本操作
// ============================================================================

#include "core/Paragraph.h"
#include "core/TextRun.h"
#include "core/MathObject.h"

/**
 * @brief 构造函数
 * 创建空段落
 */
Paragraph::Paragraph() {}

/**
 * @brief 向段落末尾添加文本
 * 
 * 该方法创建一个文本片段项，并将其添加到段落末尾
 * 
 * @param text 要添加的文本内容
 * @param format 文本格式
 */
void Paragraph::appendText(const QString &text, const Format &format) {
    Item item;
    item.type = TextRunItem;
    item.data = QVariant::fromValue(TextRun(text, format));
    m_items.append(item);
}

/**
 * @brief 向段落末尾添加数学对象
 * 
 * 该方法创建一个数学对象项，并将其添加到段落末尾
 * 
 * @param obj 要添加的数学对象
 */
void Paragraph::appendMathObject(const MathObject &obj) {
    Item item;
    item.type = MathObjectItem;
    item.data = QVariant::fromValue(obj);
    m_items.append(item);
}

/**
 * @brief 获取段落中的项数量
 * @return 项的总数
 */
int Paragraph::itemCount() const { return m_items.size(); }

/**
 * @brief 获取指定位置的项（常量引用）
 * @param index 项的索引
 * @return 指定项的常量引用
 */
const Paragraph::Item &Paragraph::itemAt(int index) const { return m_items[index]; }

/**
 * @brief 获取指定位置的项（可修改引用）
 * @param index 项的索引
 * @return 指定项的可修改引用
 */
Paragraph::Item &Paragraph::itemAt(int index) { return m_items[index]; }

/**
 * @brief 在指定位置插入项
 * @param index 插入位置
 * @param item 要插入的项
 */
void Paragraph::insertItem(int index, const Item &item) { m_items.insert(index, item); }

/**
 * @brief 获取段落的纯文本表示
 * 
 * 该方法将段落中的所有文本内容拼接成一个字符串，数学对象用"[Math]"表示
 * 主要用于调试目的
 * 
 * @return 段落中所有文本内容的拼接字符串
 */
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