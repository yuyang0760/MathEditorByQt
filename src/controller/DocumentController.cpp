#include "controller/DocumentController.h"
#include "core/Format.h"
#include "core/TextRun.h"

DocumentController::DocumentController(QObject *parent)
    : QObject(parent), m_document(nullptr) {}

void DocumentController::setDocument(Document *document) {
    m_document = document;
}

Document *DocumentController::document() const { return m_document; }

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

void DocumentController::replaceText(const Selection &selection, const QString &text) {
    if (!m_document) return;
    
    // 简化实现：删除选择内容，然后插入新文本
    deleteText(selection);
    insertText(selection.start(), text);
}

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

void DocumentController::insertParagraph(int paragraphIndex) {
    if (!m_document) return;
    
    Paragraph newPara;
    m_document->insertParagraph(paragraphIndex, newPara);
    emit documentChanged();
}