/*! \file TextEditorWidget.h
    \brief 文本编辑器部件头文件
    \author 项目开发者
    \date 2026-02-14
    
    \details 提供文本编辑器的主要界面和功能集成
*/

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

/*! \class TextEditorWidget
    \brief 文本编辑器部件类
    
    TextEditorWidget是应用程序的主界面部件，负责集成文档视图、控制器和状态栏。
    它提供完整的文本编辑功能，包括键盘输入、输入法支持、选择操作和状态显示。
    
    \section editor_features 主要功能
    - 集成文档视图显示
    - 键盘事件处理
    - 输入法支持
    - 状态栏信息显示
    - 鼠标位置跟踪
    
    \section editor_usage 使用示例
    \code
    TextEditorWidget *editor = new TextEditorWidget();
    Document *doc = new Document();
    editor->setDocument(doc);
    \endcode
*/
class TextEditorWidget : public QWidget {
    Q_OBJECT
public:
    /*! \brief 构造函数
        
        \param parent 父部件指针
    */
    TextEditorWidget(QWidget *parent = nullptr);

    /*! \brief 设置文档对象
        
        \param document 要设置的文档指针
    */
    void setDocument(Document *document);
    
    /*! \brief 获取当前文档对象
        
        \return 当前文档指针
    */
    Document *document() const;

protected:
    /*! \brief 键盘按下事件处理
        
        \param event 键盘事件对象
    */
    void keyPressEvent(QKeyEvent *event) override;
    
    /*! \brief 输入法事件处理
        
        \param event 输入法事件对象
    */
    void inputMethodEvent(QInputMethodEvent *event) override;
    
    /*! \brief 输入法查询处理
        
        \param query 查询类型
        \return 查询结果
    */
    QVariant inputMethodQuery(Qt::InputMethodQuery query) const override;

private slots:
    /*! \brief 选择改变槽函数
        
        \param selection 新的选择对象
    */
    void onSelectionChanged(const Selection &selection);
    
    void onStyleSelected(int index);          // 新增：样式选择
    void onStylesChanged();                   // 新增：样式列表变化

    // 直接格式工具栏槽函数
    void onFontFamilyChanged(const QFont &font);
    void onFontSizeChanged(const QString &size);
    void onBoldToggled(bool checked);
    void onItalicToggled(bool checked);
    void onUnderlineToggled(bool checked);
    void onColorClicked();

private:
    /*! \brief 初始化用户界面
    */
    void setupUI();
    
    /*! \brief 建立信号槽连接
    */
    void setupConnections();
    
    void updateStyleCombo();                   // 新增：更新样式下拉框
    void updateFormatToolbar(const Format &format); // 新增：更新格式工具栏
    Format getSelectionFormat(const Selection &selection); // 新增：获取选择格式
    Format getCharacterFormatBeforeCursor(); // 新增：获取光标前字符的格式
    QString getSelectedText(const Selection &selection); // 新增：获取选中文字

    QVBoxLayout *m_layout;
    QToolBar *m_toolBar;                       // 新增：工具栏
    QComboBox *m_styleCombo;                   // 新增：样式下拉框
    QToolBar *m_formatToolBar;                 // 新增：格式工具栏
    QFontComboBox *m_fontCombo;                // 新增：字体选择框
    QComboBox *m_fontSizeCombo;                // 新增：字号选择框
    QAction *m_boldAction;                     // 新增：粗体按钮
    QAction *m_italicAction;                   // 新增：斜体按钮
    QAction *m_underlineAction;                // 新增：下划线按钮
    QPushButton *m_colorButton;                // 新增：颜色按钮
    DocumentView *m_documentView;

    DocumentController *m_documentController;
    SelectionController *m_selectionController;
    InputController *m_inputController;

    Document *m_document;
    
    bool m_updatingFromSelection;               // 新增：是否正在从选择更新工具栏
    Format m_lastSelectionFormat;               // 新增：最后选择的格式，用于状态栏显示
};

#endif // TEXTEDITORWIDGET_H