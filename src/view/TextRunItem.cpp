// ============================================================================
// TextRunItem.cpp
// 文本片段图形项实现文件
// 实现文本片段的图形化显示，支持文本格式的应用
// ============================================================================

#include "view/TextRunItem.h"
#include <QTextDocument>

/**
 * @brief 构造函数
 * 
 * 初始化文本片段图形项，设置父项、文本内容和格式
 * 
 * @param run 文本片段
 * @param parent 父图形项
 */
TextRunItem::TextRunItem(const TextRun &run, QGraphicsItem *parent)
    : QGraphicsTextItem(run.text(), parent), m_run(run)
{
    setFont(run.format().font());
    setDefaultTextColor(run.format().color());
    
    // 关键修复：移除文档边距，使文本从 (0,0) 开始绘制
    QTextDocument *doc = document();
    if (doc) {
        doc->setDocumentMargin(0);
    }
}

/**
 * @brief 设置文本片段
 * 
 * 更新文本片段数据，并重新设置文本内容和格式
 * 
 * @param run 新的文本片段
 */
void TextRunItem::setRun(const TextRun &run) {
    m_run = run;
    setPlainText(run.text());
    setFont(run.format().font());
    setDefaultTextColor(run.format().color());
    
    // 关键修复：移除文档边距，使文本从 (0,0) 开始绘制
    QTextDocument *doc = document();
    if (doc) {
        doc->setDocumentMargin(0);
    }
}

/**
 * @brief 获取当前文本片段
 * 
 * @return 当前文本片段
 */
TextRun TextRunItem::run() const { return m_run; }