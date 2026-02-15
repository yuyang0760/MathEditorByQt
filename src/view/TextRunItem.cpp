// ============================================================================
// TextRunItem.cpp
// 文本片段图形项实现文件
// 实现文本片段的图形化显示，支持文本格式的应用
// ============================================================================

#include "view/TextRunItem.h"
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include "core/StyleManager.h"
#include "core/CharacterFormat.h"

/**
 * @brief 构造函数
 * 
 * 初始化文本片段图形项，设置父项、文本内容和格式
 * 
 * @param run 文本片段
 * @param parent 父图形项
 */
TextRunItem::TextRunItem(const TextRun &run, int paragraphIndex, int itemIndex, int offsetStart, int offsetEnd, QGraphicsItem *parent)
    : QGraphicsTextItem(run.text(), parent), m_run(run), 
      m_paragraphIndex(paragraphIndex), m_itemIndex(itemIndex), 
      m_offsetStart(offsetStart), m_offsetEnd(offsetEnd)
{
    StyleManager *styleMgr = StyleManager::instance();
    CharacterFormat eff = m_run.effectiveFormat(styleMgr);
    if (eff.hasFontFamily() && eff.hasFontSize()) {
        setFont(eff.toFont());
    }
    if (eff.hasColor()) {
        setDefaultTextColor(eff.color());
    }
    
    // 关键修复：移除文档边距，使文本从 (0,0) 开始绘制
    QTextDocument *doc = document();
    if (doc) {
        doc->setDocumentMargin(0);
    }
    
    // 禁用 QGraphicsTextItem 的默认选择功能
    setTextInteractionFlags(Qt::NoTextInteraction);
    
    // 确保初始状态下没有任何选择高亮 - 彻底清除
    QTextCursor cursor = textCursor();
    cursor.select(QTextCursor::Document);
    QTextCharFormat clearFormat;
    clearFormat.setBackground(Qt::transparent);
    clearFormat.clearForeground();
    cursor.setCharFormat(clearFormat);
    cursor.clearSelection();
    setTextCursor(cursor);
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
    StyleManager *styleMgr = StyleManager::instance();
    CharacterFormat eff = m_run.effectiveFormat(styleMgr);
    if (eff.hasFontFamily() && eff.hasFontSize()) {
        setFont(eff.toFont());
    }
    if (eff.hasColor()) {
        setDefaultTextColor(eff.color());
    }
    
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

int TextRunItem::paragraphIndex() const { return m_paragraphIndex; }

int TextRunItem::itemIndex() const { return m_itemIndex; }

int TextRunItem::offsetStart() const { return m_offsetStart; }

int TextRunItem::offsetEnd() const { return m_offsetEnd; }

/**
 * @brief 设置选中状态
 * 
 * 设置文本的选中状态和背景颜色
 * 
 * @param selected 是否选中
 * @param start 选中起始位置
 * @param end 选中结束位置
 */
void TextRunItem::setSelected(bool selected, int start, int end) {
    QTextCursor cursor = textCursor();
    
    // 彻底重置所有文本格式 - 确保完全清除
    cursor.select(QTextCursor::Document);
    QTextCharFormat clearFormat;
    clearFormat.setBackground(Qt::transparent);
    clearFormat.clearForeground();
    cursor.setCharFormat(clearFormat);
    cursor.clearSelection();
    
    if (selected) {
        // 计算实际的结束位置
        int actualEnd = (end == -1) ? m_run.length() : end;
        actualEnd = qMin(actualEnd, m_run.length());
        start = qMax(0, start);
        
        if (start < actualEnd) {
            // 设置选中部分的背景颜色
            cursor.setPosition(start);
            cursor.setPosition(actualEnd, QTextCursor::KeepAnchor);
            QTextCharFormat selectedFormat;
            selectedFormat.setBackground(Qt::blue);
            selectedFormat.setForeground(Qt::white);
            cursor.setCharFormat(selectedFormat);
        }
    }
    
    setTextCursor(cursor);
}
