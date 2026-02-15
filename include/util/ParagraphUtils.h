// ============================================================================
// ParagraphUtils.h
// 段落工具类头文件
// 提供段落操作的通用工具方法，减少代码重复
// ============================================================================

#ifndef PARAGRAPHUTILS_H
#define PARAGRAPHUTILS_H

#include "core/Paragraph.h"
#include "core/TextRun.h"
#include "core/Format.h"
#include <QList>

/**
 * @brief 段落工具类
 * 
 * ParagraphUtils类提供段落操作的通用工具方法，主要用于减少DocumentController中
 * 格式应用操作的代码重复。
 */
class ParagraphUtils {
public:
    /**
     * @brief 应用格式到段落的选择范围
     * 
     * 该方法将指定的格式操作应用到段落的选择范围内，支持样式和直接格式两种方式。
     * 处理复杂的跨项分割和合并逻辑。
     * 
     * @param para 要操作的段落
     * @param itemStart 起始项索引
     * @param offsetStart 起始项内偏移
     * @param itemEnd 结束项索引
     * @param offsetEnd 结束项内偏移
     * @param formatOperation 格式操作函数，接受TextRun引用并应用格式
     */
    static void applyFormatToSelection(Paragraph &para, 
                                      int itemStart, int offsetStart,
                                      int itemEnd, int offsetEnd,
                                      std::function<void(TextRun&)> formatOperation);

    /**
     * @brief 创建文本项
     * 
     * 辅助方法，用于创建Paragraph::Item对象
     * 
     * @param run 文本运行对象
     * @return 创建的文本项
     */
    static Paragraph::Item createTextItem(const TextRun &run);

    /**
     * @brief 分割文本运行
     * 
     * 将文本运行按照指定的偏移分割为多个部分
     * 
     * @param run 要分割的文本运行
     * @param splitOffset 分割偏移位置
     * @param before 分割前的部分（输出参数）
     * @param after 分割后的部分（输出参数）
     */
    static void splitTextRun(const TextRun &run, int splitOffset, 
                            TextRun &before, TextRun &after);

    /**
     * @brief 提取文本运行的子部分
     * 
     * 从文本运行中提取指定范围的子部分
     * 
     * @param run 源文本运行
     * @param start 起始偏移
     * @param length 提取长度
     * @return 提取的子文本运行
     */
    static TextRun extractTextRunSubset(const TextRun &run, int start, int length);
};

#endif // PARAGRAPHUTILS_H