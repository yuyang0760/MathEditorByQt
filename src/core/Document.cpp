#include "core/Document.h"

Document::Document() {}

void Document::addParagraph(const Paragraph &paragraph) { m_paragraphs.append(paragraph); }
void Document::insertParagraph(int index, const Paragraph &paragraph) { m_paragraphs.insert(index, paragraph); }
void Document::removeParagraph(int index) { m_paragraphs.removeAt(index); }
Paragraph Document::paragraph(int index) const { return m_paragraphs[index]; }
Paragraph &Document::paragraph(int index) { return m_paragraphs[index]; }
int Document::paragraphCount() const { return m_paragraphs.size(); }
void Document::clear() { m_paragraphs.clear(); }