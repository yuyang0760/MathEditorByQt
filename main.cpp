// ============================================================================
// main.cpp
// 主函数文件
// 应用程序的入口点，负责创建主窗口和初始化编辑器
// ============================================================================

#include "view/TextEditorWidget.h"
#include "core/Document.h"
#include "core/Paragraph.h"
#include "core/Run.h"
#include <QApplication>
#include <QMainWindow>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QLabel>
#include <QDebug>

/**
 * @brief 主函数
 * @param argc 命令行参数个数
 * @param argv 命令行参数数组
 * @return 应用程序退出代码
 */
int main(int argc, char *argv[])
{
    qDebug() << "Starting application...";
    
    // 创建应用程序实例
    QApplication a(argc, argv);
    
    qDebug() << "Creating main window...";
    // 创建主窗口
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("MathEditor");
    mainWindow.resize(800, 600);
    
    qDebug() << "Creating status bar...";
    // 创建状态栏
    QStatusBar *statusBar = mainWindow.statusBar();
    
    // 创建鼠标坐标显示标签
    QLabel *mousePositionLabel = new QLabel("鼠标坐标: (0, 0)");
    statusBar->addPermanentWidget(mousePositionLabel);
    
    // 创建文档状态显示标签
    QLabel *documentStatusLabel = new QLabel("就绪");
    statusBar->addWidget(documentStatusLabel);
    
    qDebug() << "Creating menu bar...";
    // 创建菜单栏
    QMenuBar *menuBar = mainWindow.menuBar();
    
    // 文件菜单
    QMenu *fileMenu = menuBar->addMenu("文件");
    QAction *newAction = fileMenu->addAction("新建");
    QAction *openAction = fileMenu->addAction("打开");
    QAction *saveAction = fileMenu->addAction("保存");
    fileMenu->addSeparator();
    QAction *exitAction = fileMenu->addAction("退出");
    
    // 编辑菜单
    QMenu *editMenu = menuBar->addMenu("编辑");
    QAction *undoAction = editMenu->addAction("撤销");
    QAction *redoAction = editMenu->addAction("重做");
    editMenu->addSeparator();
    QAction *cutAction = editMenu->addAction("剪切");
    QAction *copyAction = editMenu->addAction("复制");
    QAction *pasteAction = editMenu->addAction("粘贴");
    editMenu->addSeparator();
    QAction *selectAllAction = editMenu->addAction("全选");
    
    qDebug() << "Creating document...";
    // 创建文档
    Document *document = new Document();
    
    qDebug() << "Adding paragraphs...";
    // 添加一些示例文本
    Paragraph paragraph1;
    paragraph1.setText("Hello, this is a test document.");
    document->addParagraph(paragraph1);
    
    Paragraph paragraph2;
    paragraph2.setText("This is the second paragraph.");
    document->addParagraph(paragraph2);
    
    qDebug() << "Creating TextEditorWidget...";
    // 创建文本编辑器部件
    TextEditorWidget *editorWidget = new TextEditorWidget();
    editorWidget->setDocument(document);
    
    qDebug() << "Setting central widget...";
    // 设置中央部件
    mainWindow.setCentralWidget(editorWidget);
    
    // 连接信号槽
    QObject::connect(exitAction, &QAction::triggered, &a, &QApplication::quit);
    
    // 连接鼠标位置更新信号
    QObject::connect(editorWidget->documentView(), &DocumentView::mousePositionChanged, 
                     [mousePositionLabel](const QPointF& scenePos, const QPoint& viewPos) {
                         QString text = QString("鼠标坐标: 场景(%1, %2) 视图(%3, %4)")
                                       .arg(scenePos.x()).arg(scenePos.y())
                                       .arg(viewPos.x()).arg(viewPos.y());
                         mousePositionLabel->setText(text);
                     });
    
    qDebug() << "Showing main window...";
    // 显示主窗口
    mainWindow.show();
    
    qDebug() << "Entering event loop...";
    // 进入事件循环
    int result = a.exec();
    
    qDebug() << "Exiting event loop with result:" << result;
    // 清理资源
    delete document;
    
    qDebug() << "Application exiting...";
    return result;
}
