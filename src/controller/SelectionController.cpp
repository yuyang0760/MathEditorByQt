#include "controller/SelectionController.h"

SelectionController::SelectionController(QObject *parent)
    : QObject(parent), m_selection() {}

void SelectionController::setSelection(const Selection &selection) {
    if (m_selection != selection) {
        m_selection = selection;
        emit selectionChanged(m_selection);
    }
}

Selection SelectionController::selection() const { return m_selection; }

bool SelectionController::hasSelection() const { return !m_selection.isEmpty(); }