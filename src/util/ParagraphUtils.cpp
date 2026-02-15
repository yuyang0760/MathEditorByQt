// ============================================================================
// ParagraphUtils.cpp
// 段落工具类实现文件
// 实现段落操作的通用工具方法
// ============================================================================

#include "util/ParagraphUtils.h"
#include <functional>

/**
 * @brief 应用格式到段落的选择范围
 */
void ParagraphUtils::applyFormatToSelection(Paragraph &para, 
                                           int itemStart, int offsetStart,
                                           int itemEnd, int offsetEnd,
                                           std::function<void(TextRun&)> formatOperation) {
    QList<Paragraph::Item> newItems;
    
    for (int i = 0; i < para.itemCount(); ++i) {
        auto &item = para.itemAt(i);
        
        if (item.type == Paragraph::TextRunItem) {
            TextRun run = item.data.value<TextRun>();
            
            if (i >= itemStart && i <= itemEnd) {
                // 在选择范围内的项
                if (i == itemStart && i == itemEnd) {
                    // 同一项内部分选择
                    if (offsetStart > 0 || offsetEnd < run.length()) {
                        // 需要分割项
                        if (offsetStart > 0) {
                            // 前半部分保持原格式
                            TextRun before = extractTextRunSubset(run, 0, offsetStart);
                            newItems.append(createTextItem(before));
                        }
                        
                        // 中间部分应用新格式
                        TextRun middle = extractTextRunSubset(run, offsetStart, offsetEnd - offsetStart);
                        formatOperation(middle);
                        newItems.append(createTextItem(middle));
                        
                        if (offsetEnd < run.length()) {
                            // 后半部分保持原格式
                            TextRun after = extractTextRunSubset(run, offsetEnd, run.length() - offsetEnd);
                            newItems.append(createTextItem(after));
                        }
                    } else {
                        // 整个项被选中
                        formatOperation(run);
                        newItems.append(createTextItem(run));
                    }
                } else if (i == itemStart) {
                    // 起始项
                    if (offsetStart > 0) {
                        // 前半部分保持原格式
                        TextRun before = extractTextRunSubset(run, 0, offsetStart);
                        newItems.append(createTextItem(before));
                    }
                    
                    // 后半部分应用新格式
                    TextRun after = extractTextRunSubset(run, offsetStart, run.length() - offsetStart);
                    formatOperation(after);
                    newItems.append(createTextItem(after));
                } else if (i == itemEnd) {
                    // 结束项
                    // 前半部分应用新格式
                    TextRun before = extractTextRunSubset(run, 0, offsetEnd);
                    formatOperation(before);
                    newItems.append(createTextItem(before));
                    
                    if (offsetEnd < run.length()) {
                        // 后半部分保持原格式
                        TextRun after = extractTextRunSubset(run, offsetEnd, run.length() - offsetEnd);
                        newItems.append(createTextItem(after));
                    }
                } else {
                    // 中间完整项
                    formatOperation(run);
                    newItems.append(createTextItem(run));
                }
            } else {
                // 不在选择范围内的项，保持原样
                newItems.append(item);
            }
        } else {
            // 数学对象，保持原样
            newItems.append(item);
        }
    }
    
    // 替换整个段落的内容
    para.replaceItems(0, para.itemCount(), newItems);
}

/**
 * @brief 创建文本项
 */
Paragraph::Item ParagraphUtils::createTextItem(const TextRun &run) {
    Paragraph::Item item;
    item.type = Paragraph::TextRunItem;
    item.data = QVariant::fromValue(run);
    return item;
}

/**
 * @brief 分割文本运行
 */
void ParagraphUtils::splitTextRun(const TextRun &run, int splitOffset, 
                                 TextRun &before, TextRun &after) {
    before = extractTextRunSubset(run, 0, splitOffset);
    after = extractTextRunSubset(run, splitOffset, run.length() - splitOffset);
}

/**
 * @brief 提取文本运行的子部分
 */
TextRun ParagraphUtils::extractTextRunSubset(const TextRun &run, int start, int length) {
    TextRun subset = run;
    subset.setText(run.text().mid(start, length));
    return subset;
}