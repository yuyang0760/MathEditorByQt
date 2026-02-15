// ============================================================================
// ParagraphLayout.cpp
// 段落布局类实现文件
// ============================================================================

#include "view/ParagraphLayout.h"
#include "core/CharacterFormat.h"
#include "core/TextRun.h"
#include "core/StyleManager.h"
#include <QFontMetrics>
#include <QTextBoundaryFinder>

/**
 * @brief 构造函数
 */
ParagraphLayout::ParagraphLayout(QObject *parent)
    : QObject(parent), 
      m_paragraph(nullptr), 
      m_paragraphIndex(0), 
      m_maxWidth(800), 
      m_dirty(true),
      m_height(0) {}

/**
 * @brief 设置段落内容
 */
void ParagraphLayout::setParagraph(const Paragraph *paragraph) {
    if (m_paragraph != paragraph) {
        m_paragraph = paragraph;
        markDirty();
    }
}

/**
 * @brief 设置段落索引
 */
void ParagraphLayout::setParagraphIndex(int index) {
    if (m_paragraphIndex != index) {
        m_paragraphIndex = index;
        markDirty();
    }
}

/**
 * @brief 获取段落索引
 */
int ParagraphLayout::paragraphIndex() const {
    return m_paragraphIndex;
}

/**
 * @brief 设置最大宽度
 */
void ParagraphLayout::setMaxWidth(qreal width) {
    if (!qFuzzyCompare(m_maxWidth, width)) {
        m_maxWidth = width;
        markDirty();
    }
}

/**
 * @brief 获取最大宽度
 */
qreal ParagraphLayout::maxWidth() const {
    return m_maxWidth;
}

/**
 * @brief 标记布局为脏，需要重新计算
 */
void ParagraphLayout::markDirty() {
    m_dirty = true;
}

/**
 * @brief 检查是否需要重新布局
 */
bool ParagraphLayout::isDirty() const {
    return m_dirty;
}

/**
 * @brief 清除布局
 */
void ParagraphLayout::clear() {
    m_lines.clear();
    m_height = 0;
    m_dirty = true;
}

/**
 * @brief 计算布局
 */
void ParagraphLayout::layout() {
    if (!m_dirty || !m_paragraph) {
        return;
    }
    
    m_lines.clear();
    m_height = 0;
    
    if (m_paragraph->itemCount() == 0) {
        // 空段落，创建一个空行
        Line emptyLine(m_paragraphIndex, 0);
        emptyLine.setRect(QRectF(0, 0, 0, 20));
        emptyLine.setBaseline(16);
        Position startPos = {m_paragraphIndex, 0, 0};
        emptyLine.setStartPosition(startPos);
        emptyLine.setEndPosition(startPos);
        m_lines.append(emptyLine);
        m_height = 20;
        m_dirty = false;
        return;
    }
    
    qreal y = 0;
    qreal x = 0;
    int currentLineIndex = 0;
    Line currentLine(m_paragraphIndex, currentLineIndex);
    qreal currentLineMaxAscent = 0;
    qreal currentLineMaxDescent = 0;
    Position currentLineStart = {m_paragraphIndex, 0, 0};
    bool hasStartPosition = false;
    
    for (int itemIndex = 0; itemIndex < m_paragraph->itemCount(); ++itemIndex) {
        const Paragraph::Item &item = m_paragraph->itemAt(itemIndex);
        
        if (item.type == Paragraph::TextRunItem) {
            TextRun run = item.data.value<TextRun>();
            int textLength = run.length();
            int currentOffset = 0;
            
            while (currentOffset < textLength) {
                qreal ascent, descent;
                getTextMetrics(run, ascent, descent);
                
                // 尝试在空格处换行
                int wrapOffset = textLength;
                qreal textWidth = calculateTextWidth(run, currentOffset, wrapOffset - currentOffset);
                
                // 如果当前行加上这段文本会超过最大宽度
                if (x + textWidth > m_maxWidth && x > 0) {
                    // 尝试找到合适的换行位置
                    QString text = run.text().mid(currentOffset);
                    int lastSpace = text.lastIndexOf(' ');
                    if (lastSpace > 0) {
                        wrapOffset = currentOffset + lastSpace + 1;
                        textWidth = calculateTextWidth(run, currentOffset, wrapOffset - currentOffset);
                    } else {
                        // 没有空格，强制换行
                        wrapOffset = currentOffset;
                    }
                    
                    if (wrapOffset > currentOffset) {
                        // 有文本可以添加到当前行
                        LineSegment segment(itemIndex, currentOffset, wrapOffset, 
                                           x, textWidth, ascent, descent);
                        currentLine.addSegment(segment);
                        
                        if (!hasStartPosition) {
                            currentLineStart = {m_paragraphIndex, itemIndex, currentOffset};
                            hasStartPosition = true;
                        }
                        
                        x += textWidth;
                        currentLineMaxAscent = qMax(currentLineMaxAscent, ascent);
                        currentLineMaxDescent = qMax(currentLineMaxDescent, descent);
                        currentOffset = wrapOffset;
                    }
                    
                    // 结束当前行
                    qreal lineHeight = currentLineMaxAscent + currentLineMaxDescent;
                    currentLine.setRect(QRectF(0, y, x, lineHeight));
                    currentLine.setBaseline(currentLineMaxAscent);
                    currentLine.setStartPosition(currentLineStart);
                    
                    Position endPos = {m_paragraphIndex, itemIndex, currentOffset};
                    currentLine.setEndPosition(endPos);
                    
                    m_lines.append(currentLine);
                    m_height += lineHeight;
                    y += lineHeight;
                    
                    // 开始新行
                    currentLineIndex++;
                    currentLine = Line(m_paragraphIndex, currentLineIndex);
                    x = 0;
                    currentLineMaxAscent = 0;
                    currentLineMaxDescent = 0;
                    hasStartPosition = false;
                }
                
                if (currentOffset < textLength) {
                    // 添加剩余文本到当前行
                    wrapOffset = textLength;
                    textWidth = calculateTextWidth(run, currentOffset, wrapOffset - currentOffset);
                    
                    LineSegment segment(itemIndex, currentOffset, wrapOffset, 
                                       x, textWidth, ascent, descent);
                    currentLine.addSegment(segment);
                    
                    if (!hasStartPosition) {
                        currentLineStart = {m_paragraphIndex, itemIndex, currentOffset};
                        hasStartPosition = true;
                    }
                    
                    x += textWidth;
                    currentLineMaxAscent = qMax(currentLineMaxAscent, ascent);
                    currentLineMaxDescent = qMax(currentLineMaxDescent, descent);
                    currentOffset = wrapOffset;
                }
            }
        }
    }
    
    // 添加最后一行
    if (!currentLine.segments().isEmpty() || m_lines.isEmpty()) {
        qreal lineHeight = currentLineMaxAscent + currentLineMaxDescent;
        if (lineHeight <= 0) {
            lineHeight = 20;
            currentLineMaxAscent = 16;
        }
        
        currentLine.setRect(QRectF(0, y, x, lineHeight));
        currentLine.setBaseline(currentLineMaxAscent);
        
        if (!hasStartPosition) {
            currentLineStart = {m_paragraphIndex, 0, 0};
        }
        currentLine.setStartPosition(currentLineStart);
        
        int lastItem = m_paragraph->itemCount() - 1;
        int lastOffset = 0;
        if (lastItem >= 0) {
            const Paragraph::Item &lastItemObj = m_paragraph->itemAt(lastItem);
            if (lastItemObj.type == Paragraph::TextRunItem) {
                TextRun lastRun = lastItemObj.data.value<TextRun>();
                lastOffset = lastRun.length();
            }
        }
        Position endPos = {m_paragraphIndex, lastItem, lastOffset};
        currentLine.setEndPosition(endPos);
        
        m_lines.append(currentLine);
        m_height += lineHeight;
    }
    
    m_dirty = false;
    emit layoutChanged();
}

/**
 * @brief 获取所有行
 */
const QList<Line> &ParagraphLayout::lines() const {
    return m_lines;
}

/**
 * @brief 获取行数
 */
int ParagraphLayout::lineCount() const {
    return m_lines.size();
}

/**
 * @brief 获取指定行
 */
const Line &ParagraphLayout::line(int index) const {
    static Line emptyLine;
    if (index >= 0 && index < m_lines.size()) {
        return m_lines[index];
    }
    return emptyLine;
}

/**
 * @brief 获取段落总高度
 */
qreal ParagraphLayout::height() const {
    return m_height;
}

/**
 * @brief 根据位置找到对应的行
 */
int ParagraphLayout::findLineForPosition(const Position &pos) const {
    for (int i = 0; i < m_lines.size(); ++i) {
        if (m_lines[i].containsPosition(pos)) {
            return i;
        }
    }
    
    // 如果找不到，根据位置估算
    if (pos < m_lines.first().startPosition()) {
        return 0;
    }
    return m_lines.size() - 1;
}

/**
 * @brief 根据点找到对应的位置
 */
Position ParagraphLayout::positionFromPoint(const QPointF &point) const {
    if (m_lines.isEmpty()) {
        return {m_paragraphIndex, 0, 0};
    }
    
    qreal searchY = point.y();
    
    // 找到包含 y 坐标的行
    int lineIndex = 0;
    for (int i = 0; i < m_lines.size(); ++i) {
        const Line &line = m_lines[i];
        if (searchY >= line.rect().top() && searchY < line.rect().bottom()) {
            lineIndex = i;
            break;
        }
        if (searchY >= line.rect().bottom()) {
            lineIndex = i;
        }
    }
    
    const Line &targetLine = m_lines[lineIndex];
    qreal localX = point.x();
    
    // 如果 x 在最左边，返回行起始位置
    if (localX <= 0) {
        return targetLine.startPosition();
    }
    
    // 如果 x 在最右边，返回行结束位置
    if (localX >= targetLine.rect().right()) {
        return targetLine.endPosition();
    }
    
    // 查找包含 x 的片段
    for (const LineSegment &segment : targetLine.segments()) {
        if (localX >= segment.x && localX <= segment.x + segment.width) {
            // 在片段内部，精确计算偏移
            if (m_paragraph && segment.itemIndex >= 0 && segment.itemIndex < m_paragraph->itemCount()) {
                const Paragraph::Item &item = m_paragraph->itemAt(segment.itemIndex);
                if (item.type == Paragraph::TextRunItem) {
                    TextRun run = item.data.value<TextRun>();
                    
                    // 逐字符计算，找到最接近的位置
                    int bestOffset = segment.offsetStart;
                    qreal bestDiff = qAbs(localX - segment.x);
                    
                    for (int testOffset = segment.offsetStart; testOffset <= segment.offsetEnd; ++testOffset) {
                        qreal testWidth = calculateTextWidth(run, segment.offsetStart, testOffset - segment.offsetStart);
                        qreal testX = segment.x + testWidth;
                        qreal diff = qAbs(localX - testX);
                        
                        if (diff < bestDiff) {
                            bestDiff = diff;
                            bestOffset = testOffset;
                        }
                    }
                    
                    return {m_paragraphIndex, segment.itemIndex, bestOffset};
                }
            }
            return {m_paragraphIndex, segment.itemIndex, segment.offsetStart};
        }
        
        // 如果 x 在当前片段之前，返回片段起始
        if (localX < segment.x) {
            return {m_paragraphIndex, segment.itemIndex, segment.offsetStart};
        }
    }
    
    return targetLine.endPosition();
}

/**
 * @brief 根据位置找到对应的点
 */
QPointF ParagraphLayout::pointFromPosition(const Position &pos) const {
    if (m_lines.isEmpty()) {
        return QPointF(0, 0);
    }
    
    int lineIndex = findLineForPosition(pos);
    if (lineIndex < 0 || lineIndex >= m_lines.size()) {
        return QPointF(0, 0);
    }
    
    const Line &line = m_lines[lineIndex];
    
    // 在该行的片段中查找
    for (const LineSegment &segment : line.segments()) {
        if (segment.itemIndex == pos.itemIndex && 
            pos.offset >= segment.offsetStart && 
            pos.offset <= segment.offsetEnd) {
            
            // 计算从片段开头到偏移位置的文本宽度
            qreal x = segment.x;
            if (pos.offset > segment.offsetStart && m_paragraph) {
                if (segment.itemIndex >= 0 && segment.itemIndex < m_paragraph->itemCount()) {
                    const Paragraph::Item &item = m_paragraph->itemAt(segment.itemIndex);
                    if (item.type == Paragraph::TextRunItem) {
                        TextRun run = item.data.value<TextRun>();
                        x = segment.x + calculateTextWidth(run, segment.offsetStart, pos.offset - segment.offsetStart);
                    }
                }
            }
            
            // 返回线的顶部 + 基线位置（与文本项的位置计算方式一致）
    return QPointF(x, line.baseline());
        }
    }
    
    // 如果没有找到，返回行的起始位置
    return QPointF(line.rect().left(), line.rect().top() + line.baseline());
}

/**
 * @brief 计算文本的宽度
 */
qreal ParagraphLayout::calculateTextWidth(const TextRun &run, int start, int length) const {
    CharacterFormat eff = run.effectiveFormat(StyleManager::instance());
    QFont defaultFont("Microsoft YaHei", 12);
    QFont font = eff.toFont(defaultFont);
    
    QFontMetricsF metrics(font);
    QString text = run.text().mid(start, length);
    return metrics.horizontalAdvance(text);
}

/**
 * @brief 获取文本的 ascent 和 descent
 */
void ParagraphLayout::getTextMetrics(const TextRun &run, qreal &ascent, qreal &descent) const {
    CharacterFormat eff = run.effectiveFormat(StyleManager::instance());
    QFont defaultFont("Microsoft YaHei", 12);
    QFont font = eff.toFont(defaultFont);
    
    QFontMetricsF metrics(font);
    ascent = metrics.ascent();
    descent = metrics.descent();
}
