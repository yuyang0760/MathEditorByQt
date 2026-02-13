#include "view/TextRunItem.h"

TextRunItem::TextRunItem(const TextRun &run, QGraphicsItem *parent)
    : QGraphicsTextItem(run.text(), parent), m_run(run)
{
    setFont(run.format().font());
    setDefaultTextColor(run.format().color());
}

void TextRunItem::setRun(const TextRun &run) {
    m_run = run;
    setPlainText(run.text());
    setFont(run.format().font());
    setDefaultTextColor(run.format().color());
}

TextRun TextRunItem::run() const { return m_run; }