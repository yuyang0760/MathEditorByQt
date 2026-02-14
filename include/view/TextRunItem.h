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
     * @param parent 父图形项
     */
    TextRunItem(const TextRun &run, QGraphicsItem *parent = nullptr);

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

private:
    TextRun m_run; ///< 内部存储的文本片段
};

#endif // TEXTRUNITEM_H