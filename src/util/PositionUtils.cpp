// ============================================================================
// PositionUtils.cpp
// 位置工具类实现文件
// 提供文档位置和字符信息的计算功能
// ============================================================================

#include "../util/PositionUtils.h"
#include "../view/DocumentView.h"
#include "core/StyleManager.h"
#include "core/CharacterFormat.h"

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
 * @brief 获取指定位置的字符信息
 * 
 * 本函数用于获取文档中指定位置的字符详细信息，包括字符本身、其在段落中的线性索引、
 * 屏幕坐标位置以及字符宽度。仅对文本段落中的 TextRunItem 类型有效。
 * 
 * @param pos      目标文档位置（段落索引、条目索引、条目内偏移）
 * @param document 文档对象指针，用于访问段落数据
 * @param view     文档视图指针，用于坐标转换
 * @param isLeft   为 true 时获取光标左侧字符；为 false 时获取右侧字符
 * @return CharInfo 结构体，包含字符有效性、字符内容、段落线性索引、屏幕坐标及宽度
 */
CharInfo PositionUtils::getCharInfo(const Position &pos, Document *document, DocumentView *view, bool isLeft) {
    CharInfo info; // 初始化返回结构，默认 valid 为 false

    // 参数合法性检查：文档与视图必须有效
    if (!document || !view) return info;

    // 检查段落索引是否越界
    const int paraCount = document->paragraphCount();
    if (pos.paragraphIndex >= paraCount) return info;

    // 获取目标段落
    const Paragraph &para = document->paragraph(pos.paragraphIndex);
    const int itemCount = para.itemCount();

    // 检查条目索引是否越界
    if (pos.itemIndex >= itemCount) return info;

    // 仅支持 TextRunItem 类型，其他类型（如公式）直接返回空信息
    const auto &item = para.itemAt(pos.itemIndex);
    if (item.type != Paragraph::TextRunItem) return info;

    // 取出 TextRun 数据
    const TextRun textRun = item.data.value<TextRun>();

    // 根据 isLeft 决定取左侧还是右侧字符索引
    const int charIndex = isLeft ? pos.offset - 1 : pos.offset;

    // 确保字符索引在合法范围内
    if (charIndex < 0 || charIndex >= textRun.text().length()) return info;

    // 填充有效标志
    info.valid = true;

    // 记录字符本身
    info.ch = textRun.text().at(charIndex);

    // 计算该字符在整个段落中的线性字符索引（用于后续行号计算等）
    info.currentLineIndex = currentParagraphCharIndex({pos.paragraphIndex, pos.itemIndex, charIndex}, document);

    // 计算字符在视图中的屏幕坐标
    info.pos = view->pointFromPosition({pos.paragraphIndex, pos.itemIndex, charIndex});

    // 计算字符宽度，使用 TextRun 的字体信息
    StyleManager *styleMgr = StyleManager::instance();
    CharacterFormat eff = textRun.effectiveFormat(styleMgr);
    QFont font;
    if (eff.hasFontFamily() && eff.hasFontSize()) {
        font = eff.toFont();
    } else {
        font = QFont("Microsoft YaHei", 12);
    }
    info.width = QFontMetricsF(font).horizontalAdvance(info.ch);

    return info;
}