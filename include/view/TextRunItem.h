// ============================================================================
// TextRunItem.h
// 文本片段图形项头文件
// 负责文本片段的图形化显示
// ============================================================================

#ifndef TEXTRUNITEM_H
#define TEXTRUNITEM_H

#include <QGraphicsTextItem>
#include "core/TextRun.h"

/**
 * @brief 文本片段图形项类
 * 
 * TextRunItem类负责文本片段的图形化显示，继承自QGraphicsTextItem。
 * 它将文本片段渲染为图形项，在文档视图中显示，并支持文本格式的应用。
 * 
 * 主要功能：
 * - 文本片段的图形化显示
 * - 文本格式的应用
 * - 文本片段数据管理
 */
class TextRunItem : public QGraphicsTextItem {
public:
    /**
     * @brief 构造函数
     * @param run 文本片段
     * @param paragraphIndex 段落索引
     * @param itemIndex 项索引
     * @param offsetStart 片段起始偏移
     * @param offsetEnd 片段结束偏移
     * @param parent 父图形项
     */
    TextRunItem(const TextRun &run, int paragraphIndex, int itemIndex, int offsetStart, int offsetEnd, QGraphicsItem *parent = nullptr);

    /**
     * @brief 设置文本片段
     * @param run 新的文本片段
     */
    void setRun(const TextRun &run);
    
    /**
     * @brief 获取当前文本片段
     * @return 当前文本片段
     */
    TextRun run() const;
    
    /**
     * @brief 设置选中状态
     * @param selected 是否选中
     * @param start 选中起始位置（默认为0）
     * @param end 选中结束位置（默认为-1，表示到文本末尾）
     */
    void setSelected(bool selected, int start = 0, int end = -1);

    /**
     * @brief 获取段落索引
     */
    int paragraphIndex() const;
    
    /**
     * @brief 获取项索引
     */
    int itemIndex() const;
    
    /**
     * @brief 获取片段起始偏移
     */
    int offsetStart() const;
    
    /**
     * @brief 获取片段结束偏移
     */
    int offsetEnd() const;

private:
    TextRun m_run; ///< 内部存储的文本片段
    int m_paragraphIndex; ///< 段落索引
    int m_itemIndex; ///< 项索引
    int m_offsetStart; ///< 片段起始偏移
    int m_offsetEnd; ///< 片段结束偏移
};

#endif // TEXTRUNITEM_H