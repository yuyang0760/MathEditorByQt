#ifndef DOCUMENTCONTROLLER_H
#define DOCUMENTCONTROLLER_H

#include "core/Document.h"
#include "core/Selection.h"
#include <QObject>

class DocumentController : public QObject {
    Q_OBJECT
public:
    DocumentController(QObject *parent = nullptr);

    void setDocument(Document *document);
    Document *document() const;

    void insertText(const Position &position, const QString &text);
    void replaceText(const Selection &selection, const QString &text);
    void deleteText(const Selection &selection);
    void insertParagraph(int paragraphIndex);

signals:
    void documentChanged();

private:
    Document *m_document;
};

#endif // DOCUMENTCONTROLLER_H