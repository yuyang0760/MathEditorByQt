// ============================================================================
// main.cpp
// 主函数文件
// 应用程序的入口点，负责创建主窗口和初始化编辑器
// ============================================================================

#include "view/TextEditorWidget.h"
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
    qDebug() << "你好，世界！";
    // 创建应用程序实例
    QApplication a(argc, argv);
    
    qDebug() << "Creating main window...";
    // 创建主窗口
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("MathEditor");
    mainWindow.resize(800, 600);
    
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
    
    qDebug() << "Creating TextEditorWidget...";
    // 创建文本编辑器部件（内部已包含文档和状态栏）
    TextEditorWidget *editorWidget = new TextEditorWidget();
    
    qDebug() << "Setting central widget...";
    // 设置中央部件
    mainWindow.setCentralWidget(editorWidget);
    
    // 连接信号槽
    QObject::connect(exitAction, &QAction::triggered, &a, &QApplication::quit);
    
    qDebug() << "Showing main window...";
    // 显示主窗口
    mainWindow.show();
    
    qDebug() << "Entering event loop...";
    // 进入事件循环
    int result = a.exec();
    
    qDebug() << "Exiting event loop with result:" << result;
    
    qDebug() << "Application exiting...";
    return result;
}