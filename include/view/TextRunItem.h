#ifndef TEXTRUNITEM_H
#define TEXTRUNITEM_H

#include <QGraphicsTextItem>
#include "core/TextRun.h"

class TextRunItem : public QGraphicsTextItem {
public:
    TextRunItem(const TextRun &run, QGraphicsItem *parent = nullptr);

    void setRun(const TextRun &run);
    TextRun run() const;

private:
    TextRun m_run;
};

#endif // TEXTRUNITEM_H