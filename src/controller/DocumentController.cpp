// ============================================================================
// DocumentController.cpp
// 文档控制器实现文件
// 实现文档内容的编辑操作，包括文本插入、替换、删除等功能
// ============================================================================

#include "controller/DocumentController.h"
#include "core/Format.h"
#include "core/TextRun.h"

/**
 * @brief 构造函数
 * 初始化文档控制器，设置父对象和默认文档指针
 * @param parent 父对象指针
 */
DocumentController::DocumentController(QObject *parent)
    : QObject(parent), m_document(nullptr) {}

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

    if (position.paragraph >= m_document->paragraphCount()) {
        // 如果段落索引无效，追加新段落
        Paragraph newPara;
        newPara.appendText(text, Format(QFont("Microsoft YaHei", 12)));
        m_document->insertParagraph(m_document->paragraphCount(), newPara);
        emit documentChanged();
        return;
    }

    Paragraph &para = m_document->paragraph(position.paragraph);
    int itemIndex = position.item;

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
    deleteText(selection);
    insertText(selection.start(), text);
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
    if (start.paragraph != end.paragraph) {
        // 暂时忽略跨段落，或简单清除段落内容
        return;
    }

    Paragraph &para = m_document->paragraph(start.paragraph);
    if (start.item == end.item) {
        // 同一项内删除
        if (start.item >= para.itemCount()) return;
        auto &item = para.itemAt(start.item);
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