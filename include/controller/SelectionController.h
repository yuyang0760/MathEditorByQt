#ifndef SELECTIONCONTROLLER_H
#define SELECTIONCONTROLLER_H

#include "core/Selection.h"
#include <QObject>

class SelectionController : public QObject
{
    Q_OBJECT
public:
    SelectionController(QObject *parent = nullptr);
    
    // 获取和设置选择
    Selection selection() const;
    void setSelection(const Selection &selection);
    
    // 选择操作
    void selectAll();
    void selectWord(const Selection::Position &position);
    void selectParagraph(int paragraphIndex);
    
    // 扩展选择
    void extendSelection(const Selection::Position &position);
    void moveSelection(int direction, bool extend = false);
    
    // 检查选择状态
    bool hasSelection() const;
    bool isSelectionEmpty() const;
    
signals:
    // 选择变化信号
    void selectionChanged(const Selection &selection);
    
private:
    Selection m_selection;
};

#endif // SELECTIONCONTROLLER_H
