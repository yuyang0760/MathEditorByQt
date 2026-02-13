#ifndef DOCUMENT_H
#define DOCUMENT_H

#include "Paragraph.h"
#include <QVector>

class Document {
public:
    Document();

    void addParagraph(const Paragraph &paragraph);
    void insertParagraph(int index, const Paragraph &paragraph);
    void removeParagraph(int index);
    Paragraph paragraph(int index) const;
    Paragraph &paragraph(int index);
    int paragraphCount() const;

    void clear();

private:
    QVector<Paragraph> m_paragraphs;
};

#endif // DOCUMENT_H