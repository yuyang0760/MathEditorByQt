#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Paragraph.h"
#include <QVector>

class Document
{
public:
    Document();
    
    // 段落操作
    void addParagraph(const Paragraph &paragraph);
    void insertParagraph(int position, const Paragraph &paragraph);
    void removeParagraph(int position);
    Paragraph paragraph(int position) const;
    int paragraphCount() const;
    
    // 文本操作
    void insertText(int paragraphIndex, int position, const QString &text, const Format &format = Format());
    void removeText(int paragraphIndex, int position, int length);
    
    // 获取整个文档文本
    QString text() const;
    
    // 清空文档
    void clear();
    
private:
    QVector<Paragraph> m_paragraphs;
};

#endif // DOCUMENT_H
