// ============================================================================
// TextEditorWidget.h
// 文本编辑器部件头文件
// 提供文本编辑器的主要界面和功能集成
// ============================================================================

#ifndef TEXTEDITORWIDGET_H
#define TEXTEDITORWIDGET_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QToolBar>
#include <QComboBox>
#include <QFontComboBox>
#include <QPushButton>
#include <QAction>
#include <QColorDialog>
#include "view/DocumentView.h"
#include "controller/DocumentController.h"
#include "controller/SelectionController.h"
#include "controller/InputController.h"
#include "core/CharacterFormat.h"

/**
 * @brief 文本编辑器部件类
 * 
 * TextEditorWidget是应用程序的主界面部件，负责集成文档视图、控制器和状态栏。
 * 它提供完整的文本编辑功能，包括键盘输入、输入法支持、选择操作和状态显示。
 */
class TextEditorWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief 构造函数
     * @param parent 父部件指针
     */
    TextEditorWidget(QWidget *parent = nullptr);

    /**
     * @brief 设置文档对象
     * @param document 要设置的文档指针
     */
    void setDocument(Document *document);
    
    /**
     * @brief 获取当前文档对象
     * @return 当前文档指针
     */
    Document *document() const;

protected:
    /**
     * @brief 键盘按下事件处理
     * @param event 键盘事件对象
     */
    void keyPressEvent(QKeyEvent *event) override;
    
    /**
     * @brief 输入法事件处理
     * @param event 输入法事件对象
     */
    void inputMethodEvent(QInputMethodEvent *event) override;
    
    /**
     * @brief 输入法查询处理
     * @param query 查询类型
     * @return 查询结果
     */
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

private slots:
    /**
     * @brief 选择改变槽函数
     * @param selection 新的选择对象
     */
    void onSelectionChanged(const Selection &selection);
    
    void onStyleSelected(int index);
    void onStylesChanged();

    // 直接格式工具栏槽函数
    void onFontFamilyChanged(const QFont &font);
    void onFontSizeChanged(const QString &size);
    void onBoldToggled(bool checked);
    void onItalicToggled(bool checked);
    void onUnderlineToggled(bool checked);
    void onColorClicked();

private:
    /**
     * @brief 初始化用户界面
     */
    void setupUI();
    
    /**
     * @brief 建立信号槽连接
     */
    void setupConnections();
    
    void updateStyleCombo();
    void updateFormatToolbar(const CharacterFormat &format);
    CharacterFormat getSelectionFormat(const Selection &selection);
    CharacterFormat getCharacterFormatBeforeCursor();
    QString getSelectedText(const Selection &selection);

    QVBoxLayout *m_layout;
    QToolBar *m_toolBar;
    QComboBox *m_styleCombo;
    QToolBar *m_formatToolBar;
    QFontComboBox *m_fontCombo;
    QComboBox *m_fontSizeCombo;
    QAction *m_boldAction;
    QAction *m_italicAction;
    QAction *m_underlineAction;
    QPushButton *m_colorButton;
    DocumentView *m_documentView;

    DocumentController *m_documentController;
    SelectionController *m_selectionController;
    InputController *m_inputController;

    Document *m_document;
    
    bool m_updatingFromSelection;
    CharacterFormat m_lastSelectionFormat;
};

#endif // TEXTEDITORWIDGET_H
