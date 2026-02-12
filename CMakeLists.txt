# 指定所需的最低 CMake 版本
cmake_minimum_required(VERSION 3.16)

# 定义项目名称、版本号和编程语言
project(MathEditorByQt VERSION 0.1 LANGUAGES CXX)

# 启用 Qt 的自动处理功能
set(CMAKE_AUTOUIC ON)  # 自动处理 .ui 文件生成对应的头文件
set(CMAKE_AUTOMOC ON)  # 自动处理 Q_OBJECT 宏生成 moc 文件
set(CMAKE_AUTORCC ON)  # 自动处理 .qrc 资源文件

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)           # 使用 C++17 标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)  # 强制要求编译器支持指定的 C++ 标准

# 查找 Qt 包
# NAMES Qt6 Qt5: 优先查找 Qt6，找不到则查找 Qt5
# REQUIRED: 必须找到指定组件，否则构建失败
# COMPONENTS Widgets: 需要 Widgets 模块
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
# 根据找到的 Qt 版本查找对应的主要组件
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# 设置包含目录
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/include/core
    ${CMAKE_SOURCE_DIR}/include/view
    ${CMAKE_SOURCE_DIR}/include/controller
    ${CMAKE_SOURCE_DIR}/include/io
)

# 定义项目源文件列表 - 明确指定方式
# 明确列出所有源文件和头文件，确保AutoMoc能正确找到所有文件
set(PROJECT_SOURCES
    # 主文件
    main.cpp
    
    # 核心模块
    src/core/Format.cpp
    src/core/Run.cpp
    src/core/Paragraph.cpp
    src/core/Document.cpp
    src/core/Selection.cpp
    include/core/Format.h
    include/core/Run.h
    include/core/Paragraph.h
    include/core/Document.h
    include/core/Selection.h
    
    # 视图模块
    src/view/Cursor.cpp
    src/view/DocumentView.cpp
    src/view/TextEditorWidget.cpp
    include/view/Cursor.h
    include/view/DocumentView.h
    include/view/TextEditorWidget.h
    
    # 控制器模块
    src/controller/DocumentController.cpp
    src/controller/SelectionController.cpp
    src/controller/InputController.cpp
    include/controller/DocumentController.h
    include/controller/SelectionController.h
    include/controller/InputController.h
    
    # 输入输出模块
    src/io/DocumentReader.cpp
    src/io/DocumentWriter.cpp
    include/io/DocumentReader.h
    include/io/DocumentWriter.h
)

# 根据 Qt 版本和平台选择不同的构建方式
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    # Qt6 及以上版本使用新的构建命令
    qt_add_executable(MathEditorByQt
        MANUAL_FINALIZATION  # 手动控制最终化过程
        ${PROJECT_SOURCES}   # 源文件列表
    )
    # Android 平台 Qt6 的特殊配置说明（已注释）
    #    set_property(TARGET MathEditorByQt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
    #                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
    # 更多信息请参考： `https://doc.qt.io/qt-6/qt-add-executable.html#target-creation` 
else()
    # Qt5 版本的构建逻辑
    if(ANDROID)
        # Android 平台构建为共享库
        add_library(MathEditorByQt SHARED
            ${PROJECT_SOURCES}
        )
        # Android 平台 Qt5 的特殊配置说明（已注释）
        #    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        # 其他平台构建为可执行文件
        add_executable(MathEditorByQt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# 链接 Qt Widgets 模块到可执行文件
target_link_libraries(MathEditorByQt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# iOS/macOS 平台的 bundle 标识符配置
# Qt 6.1 之后会自动设置 MACOSX_BUNDLE_GUI_IDENTIFIER
# 如果开发 iOS 或 macOS 应用，建议手动设置固定的 bundle identifier
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.MathEditorByQt)
endif()

# 设置目标属性
set_target_properties(MathEditorByQt PROPERTIES
    ${BUNDLE_ID_OPTION}                                    # Bundle 标识符（仅适用于旧版本 Qt）
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}        # Bundle 版本号
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}  # 短版本号
    MACOSX_BUNDLE TRUE                                     # 创建 macOS bundle
    WIN32_EXECUTABLE TRUE                                  # Windows 平台创建 GUI 应用程序
)

# 包含 GNU 安装目录模块
include(GNUInstallDirs)

# 定义安装规则
install(TARGETS MathEditorByQt
    BUNDLE DESTINATION .                    # macOS bundle 安装路径
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}   # 库文件安装路径
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}   # 可执行文件安装路径
)

# Qt6 特有的最终化步骤
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(MathEditorByQt)
endif()
