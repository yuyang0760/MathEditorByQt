#include "core/Document.h"

Document::Document()
{
}

void Document::addParagraph(const Paragraph &paragraph)
{
    m_paragraphs.append(paragraph);
}

void Document::insertParagraph(int position, const Paragraph &paragraph)
{
    if (position >= 0 && position <= m_paragraphs.size())
    {
        m_paragraphs.insert(position, paragraph);
    }
}

void Document::removeParagraph(int position)
{
    if (position >= 0 && position < m_paragraphs.size())
    {
        m_paragraphs.removeAt(position);
    }
}

Paragraph Document::paragraph(int position) const
{
    if (position >= 0 && position < m_paragraphs.size())
    {
        return m_paragraphs[position];
    }
    return Paragraph();
}

int Document::paragraphCount() const
{
    return m_paragraphs.size();
}

void Document::insertText(int paragraphIndex, int position, const QString &text, const Format &format)
{
    if (paragraphIndex >= 0 && paragraphIndex < m_paragraphs.size())
    {
        m_paragraphs[paragraphIndex].insertText(position, text, format);
    }
}

void Document::removeText(int paragraphIndex, int position, int length)
{
    if (paragraphIndex >= 0 && paragraphIndex < m_paragraphs.size())
    {
        m_paragraphs[paragraphIndex].removeText(position, length);
    }
}

QString Document::text() const
{
    QString result;
    for (int i = 0; i < m_paragraphs.size(); i++)
    {
        result += m_paragraphs[i].text();
        if (i < m_paragraphs.size() - 1)
        {
            result += "\n";
        }
    }
    return result;
}

void Document::clear()
{
    m_paragraphs.clear();
}
