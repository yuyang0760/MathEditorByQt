// ============================================================================
// DocumentController.cpp
// 文档控制器类的实现文件
// 负责处理文档的增删改查操作，是模型和视图之间的桥梁
// ============================================================================

#include "controller/DocumentController.h"

/**
 * @brief 构造函数
 * @param parent 父对象
 */
DocumentController::DocumentController(QObject *parent)
    : QObject(parent),
      m_document(nullptr)
{
}

/**
 * @brief 设置文档
 * @param document 要设置的文档
 */
void DocumentController::setDocument(Document *document)
{
    m_document = document;
}

/**
 * @brief 获取文档
 * @return 当前文档
 */
Document *DocumentController::document() const
{
    return m_document;
}

/**
 * @brief 在指定位置插入文本
 * @param position 插入位置
 * @param text 要插入的文本
 */
void DocumentController::insertText(const Selection::Position &position, const QString &text)
{
    if (m_document)
    {
        m_document->insertText(position.paragraph, position.position, text, Format());
        emit documentChanged();
    }
}

/**
 * @brief 删除选中的文本
 * @param selection 选中的文本范围
 */
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
        emit documentChanged();
    }
}

/**
 * @brief 替换选中的文本
 * @param selection 选中的文本范围
 * @param text 替换的文本
 */
void DocumentController::replaceText(const Selection &selection, const QString &text)
{
    if (m_document)
    {
        deleteText(selection);
        insertText(selection.start(), text);
        emit documentChanged();
    }
}

/**
 * @brief 在指定位置插入段落
 * @param paragraphIndex 插入位置
 */
void DocumentController::insertParagraph(int paragraphIndex)
{
    if (m_document)
    {
        Paragraph newParagraph;
        m_document->insertParagraph(paragraphIndex, newParagraph);
        emit documentChanged();
    }
}

/**
 * @brief 删除指定段落
 * @param paragraphIndex 要删除的段落索引
 */
void DocumentController::deleteParagraph(int paragraphIndex)
{
    if (m_document)
    {
        m_document->removeParagraph(paragraphIndex);
        emit documentChanged();
    }
}

/**
 * @brief 合并指定段落和下一段落
 * @param paragraphIndex 要合并的段落索引
 */
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
        emit documentChanged();
    }
}

/**
 * @brief 应用格式到选中的文本
 * @param selection 选中的文本范围
 * @param format 要应用的格式
 */
void DocumentController::applyFormat(const Selection &selection, const Format &format)
{
    // 实现格式应用功能
    // 这里可以添加具体的格式应用逻辑
}
