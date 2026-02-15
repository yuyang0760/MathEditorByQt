// ============================================================================
// DocumentController.cpp
// 文档控制器实现文件
// 实现文档内容的编辑操作，包括文本插入、替换、删除等功能
// ============================================================================

#include "controller/DocumentController.h"
#include "core/Format.h"
#include "core/TextRun.h"
#include "core/StyleManager.h"
#include "util/ParagraphUtils.h"
#include <functional>

DocumentController::DocumentController(QObject *parent)
    : QObject(parent), m_document(nullptr), m_styleMgr(StyleManager::instance()) {}

/**
 * @brief 设置文档对象
 * @param document 要操作的文档指针
 */
void DocumentController::setDocument(Document *document) {
    m_document = document;
}

/**
 * @brief 获取当前文档对象
 * @return 当前文档指针
 */
Document *DocumentController::document() const { return m_document; }

/**
 * @brief 在指定位置插入文本
 * 
 * 该方法在文档的指定位置插入文本，支持以下情况：
 * 1. 段落索引无效时，追加新段落
 * 2. 段落为空或项索引超出时，新建文本项
 * 3. 在文本项中插入文本
 * 4. 在公式项前后插入文本
 * 
 * @param position 插入位置
 * @param text 要插入的文本
 */
void DocumentController::insertText(const Position &position, const QString &text) {
    if (!m_document || text.isEmpty()) return;

    if (position.paragraphIndex >= m_document->paragraphCount()) {
        // 如果段落索引无效，追加新段落
        Paragraph newPara;
        newPara.appendText(text, Format(QFont("Microsoft YaHei", 12)));
        m_document->insertParagraph(m_document->paragraphCount(), newPara);
        emit documentChanged();
        return;
    }

    Paragraph &para = m_document->paragraph(position.paragraphIndex);
    int itemIndex = position.itemIndex;

    // 如果段落为空或item索引超出，新建项
    if (para.itemCount() == 0 || itemIndex >= para.itemCount()) {
        para.appendText(text, Format(QFont("Microsoft YaHei", 12)));
        emit documentChanged();
        return;
    }

    auto &item = para.itemAt(itemIndex);
    if (item.type == Paragraph::TextRunItem) {
        // 在TextRun中插入
        TextRun run = item.data.value<TextRun>();
        run.insert(position.offset, text);
        item.data = QVariant::fromValue(run);
    } else { // MathObjectItem
        // 公式项：根据偏移在公式前后插入新TextRun
        Format defaultFormat(QFont("Microsoft YaHei", 12));
        Paragraph::Item newItem;
        newItem.type = Paragraph::TextRunItem;
        newItem.data = QVariant::fromValue(TextRun(text, defaultFormat));

        if (position.offset == 0) {
            // 在公式前插入
            para.insertItem(itemIndex, newItem);
        } else {
            // 在公式后插入
            para.insertItem(itemIndex + 1, newItem);
        }
    }

    emit documentChanged();
}

/**
 * @brief 替换选择区域的文本
 * 
 * 该方法通过先删除选择区域的文本，然后在起始位置插入新文本的方式实现替换功能
 * 
 * @param selection 选择区域
 * @param text 替换的文本
 */
void DocumentController::replaceText(const Selection &selection, const QString &text) {
    if (!m_document) return;
    
    // 简化实现：删除选择内容，然后插入新文本
    // 使用规范化的起始位置，确保从右向左选择时也能正确插入
    Position normStart = selection.normalizedStart();
    deleteText(selection);
    insertText(normStart, text);
}

/**
 * @brief 删除选择区域的文本
 * 
 * 该方法删除选择区域的文本，目前只支持同段落内的删除操作
 * 
 * @param selection 选择区域
 */
void DocumentController::deleteText(const Selection &selection) {
    if (!m_document) return;
    if (selection.isEmpty()) return;

    Position start = selection.normalizedStart();
    Position end = selection.normalizedEnd();

    // 如果跨段落，简化处理：只删除起始段落到结束段落？这里只实现同段落删除
    if (start.paragraphIndex != end.paragraphIndex) {
        // 暂时忽略跨段落，或简单清除段落内容
        return;
    }

    Paragraph &para = m_document->paragraph(start.paragraphIndex);
    if (start.itemIndex == end.itemIndex) {
        // 同一项内删除
        if (start.itemIndex >= para.itemCount()) return;
        auto &item = para.itemAt(start.itemIndex);
        if (item.type == Paragraph::TextRunItem) {
            TextRun run = item.data.value<TextRun>();
            int len = end.offset - start.offset;
            if (len > 0 && start.offset < run.length()) {
                run.remove(start.offset, len);
                item.data = QVariant::fromValue(run);
            }
        } // 公式项不支持内部删除
    } else {
        // 跨项删除：复杂，暂不实现
    }
    emit documentChanged();
}

/**
 * @brief 在指定位置插入段落
 * 
 * 该方法在指定索引位置插入一个新的空段落
 * 
 * @param paragraphIndex 段落插入位置
 */
void DocumentController::insertParagraph(int paragraphIndex) {
    if (!m_document) return;
    
    Paragraph newPara;
    m_document->insertParagraph(paragraphIndex, newPara);
    emit documentChanged();
}

Format DocumentController::currentDirectFormat() const {
    return m_currentDirectFormat;
}

void DocumentController::setCurrentDirectFormat(const Format &format) {
    m_currentDirectFormat = format;
    emit currentFormatChanged(format);
}

void DocumentController::applyStyle(const Selection &selection, const QString &styleId) {
    if (!m_document || selection.isEmpty()) return;

    Position start = selection.normalizedStart();
    Position end = selection.normalizedEnd();

    if (start.paragraphIndex == end.paragraphIndex) {
        // 单段落选择
        Paragraph &para = m_document->paragraph(start.paragraphIndex);
        applyStyleToParagraph(para, start.itemIndex, start.offset, end.itemIndex, end.offset, styleId);
    } else {
        // 跨段落选择
        // 处理起始段落（从选择开始到段落结束）
        Paragraph &startPara = m_document->paragraph(start.paragraphIndex);
        applyStyleToParagraph(startPara, start.itemIndex, start.offset, 
                             startPara.itemCount() - 1, 0, styleId);
        
        // 处理中间完整段落
        for (int paraIndex = start.paragraphIndex + 1; paraIndex < end.paragraphIndex; ++paraIndex) {
            Paragraph &para = m_document->paragraph(paraIndex);
            applyStyleToParagraph(para, 0, 0, para.itemCount() - 1, 0, styleId);
        }
        
        // 处理结束段落（从段落开始到选择结束）
        Paragraph &endPara = m_document->paragraph(end.paragraphIndex);
        applyStyleToParagraph(endPara, 0, 0, end.itemIndex, end.offset, styleId);
    }
    
    emit documentChanged();
}



/**
 * @brief 应用样式到段落的选择范围
 * 
 * 该方法将指定的样式应用到段落的选择范围内，使用工具类简化复杂的跨项分割逻辑。
 * 应用样式时会清除直接格式，确保样式格式生效。
 * 
 * @param para 要操作的段落
 * @param itemStart 起始项索引
 * @param offsetStart 起始项内偏移
 * @param itemEnd 结束项索引
 * @param offsetEnd 结束项内偏移
 * @param styleId 要应用的样式ID
 */
void DocumentController::applyStyleToParagraph(Paragraph &para, int itemStart, int offsetStart,
                                               int itemEnd, int offsetEnd, const QString &styleId) {
    // 使用工具类简化样式应用逻辑
    ParagraphUtils::applyFormatToSelection(para, itemStart, offsetStart, itemEnd, offsetEnd,
        [styleId](TextRun &run) {
            run.setStyleId(styleId);
            run.clearDirectFormat(); // 清除直接格式，使用样式
        });
}

/**
 * @brief 应用直接格式到选择区域
 * 
 * 该方法将指定的直接格式应用到选择区域内的文本，支持单段落和跨段落选择。
 * 直接格式会覆盖样式设置，优先级高于样式格式。
 * 
 * @param selection 要应用格式的选择区域
 * @param format 要应用的直接格式
 */
void DocumentController::applyDirectFormat(const Selection &selection, const Format &format) {
    if (!m_document || selection.isEmpty()) return;

    Position start = selection.normalizedStart();
    Position end = selection.normalizedEnd();

    if (start.paragraphIndex == end.paragraphIndex) {
        // 单段落选择
        Paragraph &para = m_document->paragraph(start.paragraphIndex);
        applyDirectFormatToParagraph(para, start.itemIndex, start.offset, end.itemIndex, end.offset, format);
    } else {
        // 跨段落选择
        // 处理起始段落（从选择开始到段落结束）
        Paragraph &startPara = m_document->paragraph(start.paragraphIndex);
        applyDirectFormatToParagraph(startPara, start.itemIndex, start.offset, 
                                    startPara.itemCount() - 1, 0, format);
        
        // 处理中间完整段落
        for (int paraIndex = start.paragraphIndex + 1; paraIndex < end.paragraphIndex; ++paraIndex) {
            Paragraph &para = m_document->paragraph(paraIndex);
            applyDirectFormatToParagraph(para, 0, 0, para.itemCount() - 1, 0, format);
        }
        
        // 处理结束段落（从段落开始到选择结束）
        Paragraph &endPara = m_document->paragraph(end.paragraphIndex);
        applyDirectFormatToParagraph(endPara, 0, 0, end.itemIndex, end.offset, format);
    }
    
    emit documentChanged();
}

/**
 * @brief 应用直接格式到段落的选择范围
 * 
 * 该方法将指定的直接格式应用到段落的选择范围内，使用工具类简化复杂的跨项分割逻辑。
 * 直接格式具有最高优先级，会覆盖样式设置。
 * 
 * @param para 要操作的段落
 * @param itemStart 起始项索引
 * @param offsetStart 起始项内偏移
 * @param itemEnd 结束项索引
 * @param offsetEnd 结束项内偏移
 * @param format 要应用的直接格式
 */
void DocumentController::applyDirectFormatToParagraph(Paragraph &para, int itemStart, int offsetStart,
                                                      int itemEnd, int offsetEnd, const Format &format) {
    // 使用工具类简化直接格式应用逻辑
    ParagraphUtils::applyFormatToSelection(para, itemStart, offsetStart, itemEnd, offsetEnd,
        [format](TextRun &run) {
            run.setDirectFormat(format); // 设置直接格式
        });
}