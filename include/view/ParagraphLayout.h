// ============================================================================
// ParagraphLayout.h
// 段落布局类头文件
// 负责计算段落的布局，管理行信息
// ============================================================================

#ifndef PARAGRAPHLAYOUT_H
#define PARAGRAPHLAYOUT_H

#include <QObject>
#include <QList>
#include <QRectF>
#include "view/Line.h"
#include "core/Paragraph.h"
#include "core/Selection.h"

/**
 * @brief 段落布局类
 * 
 * 负责计算段落的布局，将段落内容分成多行
 */
class ParagraphLayout : public QObject {
    Q_OBJECT
public:
    explicit ParagraphLayout(QObject *parent = nullptr);
    
    /**
     * @brief 设置段落内容
     * @param paragraph 段落引用
     */
    void setParagraph(const Paragraph *paragraph);
    
    /**
     * @brief 设置段落索引
     */
    void setParagraphIndex(int index);
    
    /**
     * @brief 获取段落索引
     */
    int paragraphIndex() const;
    
    /**
     * @brief 设置最大宽度
     */
    void setMaxWidth(qreal width);
    
    /**
     * @brief 获取最大宽度
     */
    qreal maxWidth() const;
    
    /**
     * @brief 计算布局
     */
    void layout();
    
    /**
     * @brief 获取所有行
     */
    const QList<Line> &lines() const;
    
    /**
     * @brief 获取行数
     */
    int lineCount() const;
    
    /**
     * @brief 获取指定行
     */
    const Line &line(int index) const;
    
    /**
     * @brief 获取段落总高度
     */
    qreal height() const;
    
    /**
     * @brief 根据位置找到对应的行
     */
    int findLineForPosition(const Position &pos) const;
    
    /**
     * @brief 根据点找到对应的位置
     */
    Position positionFromPoint(const QPointF &point) const;
    
    /**
     * @brief 根据位置找到对应的点
     */
    QPointF pointFromPosition(const Position &pos) const;
    
    /**
     * @brief 标记布局为脏，需要重新计算
     */
    void markDirty();
    
    /**
     * @brief 检查是否需要重新布局
     */
    bool isDirty() const;
    
    /**
     * @brief 清除布局
     */
    void clear();

signals:
    void layoutChanged();

private:
    /**
     * @brief 完成一行的布局计算
     */
    void finishLine(Line &line, qreal y, qreal width, 
                    qreal maxAscent, qreal maxDescent,
                    const Position &startPos,
                    int lastItemIndex, int lastOffset);
    
    /**
     * @brief 计算文本的宽度
     */
    qreal calculateTextWidth(const TextRun &run, int start, int length) const;
    
    /**
     * @brief 获取文本的 ascent 和 descent
     */
    void getTextMetrics(const TextRun &run, qreal &ascent, qreal &descent) const;
    
    const Paragraph *m_paragraph;  ///< 段落引用
    int m_paragraphIndex;           ///< 段落索引
    qreal m_maxWidth;               ///< 最大宽度
    QList<Line> m_lines;            ///< 行列表
    bool m_dirty;                   ///< 是否需要重新布局
    qreal m_height;                 ///< 段落总高度
};

#endif // PARAGRAPHLAYOUT_H
