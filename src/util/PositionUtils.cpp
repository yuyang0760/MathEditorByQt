// ============================================================================
// PositionUtils.cpp
// 位置工具类实现文件
// 提供文档位置和字符信息的计算功能
// ============================================================================

#include "../util/PositionUtils.h"
#include "../view/DocumentView.h"

/**
 * @brief 计算当前段落内的字符索引
 * 
 * 该方法计算文档位置在其段落内的字符索引，考虑了文本项和数学对象项
 * 
 * @param pos 文档位置
 * @param document 文档对象
 * @return 段落内的字符索引，无效返回-1
 */
int PositionUtils::currentParagraphCharIndex(const Position &pos, Document *document) {
    if (!document || pos.paragraphIndex >= document->paragraphCount()) return -1;
    
    int index = 0;
    const Paragraph &para = document->paragraph(pos.paragraphIndex);
    
    for (int i = 0; i < pos.itemIndex; ++i) {
        const auto &item = para.itemAt(i);
        if (item.type == Paragraph::TextRunItem) {
            TextRun textRun = item.data.value<TextRun>();
            index += textRun.text().length();
        } else {
            index += 1; // 公式项算作一个字符
        }
    }
    
    if (pos.itemIndex < para.itemCount()) {
        const auto &item = para.itemAt(pos.itemIndex);
        if (item.type == Paragraph::TextRunItem) {
            index += pos.offset;
        } else {
            index += (pos.offset > 0 ? 1 : 0); // 公式项前后位置
        }
    }
    
    return index;
}

/**
 * @brief 获取字符信息
 * 
 * 该方法获取指定位置的字符信息，包括字符内容、位置、宽度等
 * 
 * @param pos 文档位置
 * @param document 文档对象
 * @param view 文档视图
 * @param isLeft 是否获取左侧字符
 * @return 字符信息结构体
 */
CharInfo PositionUtils::getCharInfo(const Position &pos, Document *document, DocumentView *view, bool isLeft) {
    CharInfo info;
    if (!document || !view) return info;
    
    if (pos.paragraphIndex < document->paragraphCount()) {
        const Paragraph &para = document->paragraph(pos.paragraphIndex);
        if (pos.itemIndex < para.itemCount()) {
            const auto &item = para.itemAt(pos.itemIndex);
            if (item.type == Paragraph::TextRunItem) {
                TextRun textRun = item.data.value<TextRun>();
                int charIndex = isLeft ? pos.offset - 1 : pos.offset;
                if (charIndex >= 0 && charIndex < textRun.text().length()) {
                    info.valid = true;
                    info.ch = textRun.text()[charIndex];
                    info.currentLineIndex = currentParagraphCharIndex(Position{pos.paragraphIndex, pos.itemIndex, charIndex}, document);
                    
                    // 计算字符位置和宽度（简化实现）
                    QPointF basePos = view->pointFromPosition(Position{pos.paragraphIndex, pos.itemIndex, charIndex});
                    info.pos = basePos;
                    info.width = 10; // 简化宽度估计
                }
            }
        }
    }
    
    return info;
}