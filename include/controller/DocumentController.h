#ifndef DOCUMENTCONTROLLER_H
#define DOCUMENTCONTROLLER_H

#include "core/Document.h"
#include "core/Selection.h"
#include <QObject>

class DocumentController : public QObject
{
    Q_OBJECT
public:
    DocumentController(QObject *parent = nullptr);
    
    // 设置文档
    void setDocument(Document *document);
    Document *document() const;
    
    // 文档操作
    void insertText(const Selection::Position &position, const QString &text);
    void deleteText(const Selection &selection);
    void replaceText(const Selection &selection, const QString &text);
    
    // 段落操作
    void insertParagraph(int paragraphIndex);
    void deleteParagraph(int paragraphIndex);
    void mergeParagraphs(int paragraphIndex);
    
    // 格式操作
    void applyFormat(const Selection &selection, const Format &format);
    
private:
    Document *m_document;
};

#endif // DOCUMENTCONTROLLER_H
