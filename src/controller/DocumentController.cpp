#include "controller/DocumentController.h"

DocumentController::DocumentController(QObject *parent)
    : QObject(parent),
      m_document(nullptr)
{
}

void DocumentController::setDocument(Document *document)
{
    m_document = document;
}

Document *DocumentController::document() const
{
    return m_document;
}

void DocumentController::insertText(const Selection::Position &position, const QString &text)
{
    if (m_document)
    {
        m_document->insertText(position.paragraph, position.position, text);
    }
}

void DocumentController::deleteText(const Selection &selection)
{
    if (m_document && !selection.isEmpty())
    {
        Selection::Position start = selection.normalizedStart();
        Selection::Position end = selection.normalizedEnd();
        
        if (start.paragraph == end.paragraph)
        {
            // 同一段落内的删除
            int length = end.position - start.position;
            m_document->removeText(start.paragraph, start.position, length);
        }
        else
        {
            // 跨段落的删除
            // 1. 删除起始段落中从start.position到末尾的文本
            Paragraph startPara = m_document->paragraph(start.paragraph);
            int startLength = startPara.length() - start.position;
            m_document->removeText(start.paragraph, start.position, startLength);
            
            // 2. 删除中间的整个段落
            for (int i = start.paragraph + 1; i < end.paragraph; i++)
            {
                m_document->removeParagraph(start.paragraph + 1);
            }
            
            // 3. 删除结束段落中从开头到end.position的文本
            m_document->removeText(start.paragraph + 1, 0, end.position);
            
            // 4. 合并起始段落和结束段落
            mergeParagraphs(start.paragraph);
        }
    }
}

void DocumentController::replaceText(const Selection &selection, const QString &text)
{
    if (m_document)
    {
        deleteText(selection);
        insertText(selection.start(), text);
    }
}

void DocumentController::insertParagraph(int paragraphIndex)
{
    if (m_document)
    {
        Paragraph newParagraph;
        m_document->insertParagraph(paragraphIndex, newParagraph);
    }
}

void DocumentController::deleteParagraph(int paragraphIndex)
{
    if (m_document)
    {
        m_document->removeParagraph(paragraphIndex);
    }
}

void DocumentController::mergeParagraphs(int paragraphIndex)
{
    if (m_document && paragraphIndex < m_document->paragraphCount() - 1)
    {
        // 获取两个段落的文本
        Paragraph para1 = m_document->paragraph(paragraphIndex);
        Paragraph para2 = m_document->paragraph(paragraphIndex + 1);
        QString text1 = para1.text();
        QString text2 = para2.text();
        
        // 合并文本
        QString mergedText = text1 + text2;
        
        // 更新第一个段落
        para1.setText(mergedText);
        m_document->removeParagraph(paragraphIndex + 1);
    }
}

void DocumentController::applyFormat(const Selection &selection, const Format &format)
{
    // 实现格式应用功能
    // 这里可以添加具体的格式应用逻辑
}
