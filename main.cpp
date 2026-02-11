#ifdef _WIN32
#include <windows.h>
#endif

#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include "TextInputWidget.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // 基础输出
    qDebug() << "这是Qt项目的控制台输出";

    // 检测 Qt 版本
    QString versionInfo;
    QString detailedInfo;
    
    int qtMajorVersion = QT_VERSION_MAJOR;
    int qtMinorVersion = QT_VERSION_MINOR;
    int qtPatchVersion = QT_VERSION_PATCH;
    
    if (qtMajorVersion == 5) {
        versionInfo = QString("Qt5");
        detailedInfo = QString("Qt5.%1.%2").arg(qtMinorVersion).arg(qtPatchVersion);
    } else if (qtMajorVersion == 6) {
        versionInfo = QString("Qt6");
        detailedInfo = QString("Qt6.%1.%2").arg(qtMinorVersion).arg(qtPatchVersion);
    } else {
        versionInfo = QString("Qt%1").arg(qtMajorVersion);
        detailedInfo = QString("Qt%1.%2.%3").arg(qtMajorVersion).arg(qtMinorVersion).arg(qtPatchVersion);
    }
    
    QMainWindow window;


    window.setWindowTitle(QString("图形视图文本输入示例 - %1 (版本: %2)").arg(versionInfo).arg(detailedInfo));
    window.resize(800, 600);
    
    // 创建中央部件
    QWidget *centralWidget = new QWidget(&window);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    
    // 添加版本信息标签
    QLabel *versionLabel = new QLabel(QString("当前 Qt 版本: %1").arg(detailedInfo), centralWidget);
    versionLabel->setAlignment(Qt::AlignCenter);
    versionLabel->setStyleSheet("font-size: 16px; font-weight: bold; color: blue; padding: 10px;");
    layout->addWidget(versionLabel);
    
    // 添加分隔线
    QFrame *line = new QFrame(centralWidget);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    layout->addWidget(line);
    
    // 添加文本输入部件
    TextInputWidget *textInputWidget = new TextInputWidget(centralWidget);
    layout->addWidget(textInputWidget);
    
    window.setCentralWidget(centralWidget);
    
    window.show();
    
    return app.exec();
}