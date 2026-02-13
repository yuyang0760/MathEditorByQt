#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include "core/Selection.h"
#include <QObject>

class SelectionController : public QObject {
    Q_OBJECT
public:
    SelectionController(QObject *parent = nullptr);

    void setSelection(const Selection &selection);
    Selection selection() const;
    bool hasSelection() const;

signals:
    void selectionChanged(const Selection &selection);

private:
    Selection m_selection;
};

#endif // SELECTIONCONTROLLER_H