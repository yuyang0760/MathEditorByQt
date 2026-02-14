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
#include <QStatusBar>
#include <QLabel>
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
    
    /*! \brief 鼠标位置改变槽函数
        
        \param scenePos 场景坐标
        \param viewPos 视图坐标
    */
    void onMousePositionChanged(const QPointF &scenePos, const QPoint &viewPos);

private:
    /*! \brief 初始化用户界面
    */
    void setupUI();
    
    /*! \brief 建立信号槽连接
    */
    void setupConnections();
    
    /*! \brief 更新状态栏显示
    */
    void updateStatusBar();

    QVBoxLayout *m_layout;                    ///< 主布局管理器
    DocumentView *m_documentView;            ///< 文档视图组件
    QStatusBar *m_statusBar;                  ///< 状态栏组件
    QLabel *m_statusLabel;                    ///< 状态信息标签
    QLabel *m_mouseInfoLabel;                 ///< 专门显示鼠标信息的标签
    
    DocumentController *m_documentController; ///< 文档控制器
    SelectionController *m_selectionController;///< 选择控制器
    InputController *m_inputController;       ///< 输入控制器
    
    Document *m_document;                     ///< 当前文档对象
    
    QPointF m_lastMouseScenePos;             ///< 最后鼠标场景坐标
    QPoint m_lastMouseViewPos;                ///< 最后鼠标视图坐标
};

#endif // TEXTEDITORWIDGET_H