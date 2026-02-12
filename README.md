# MathEditorByQt

MathEditorByQt 是一个基于Qt框架的数学公式编辑器，旨在提供一个强大而易用的数学文档编辑环境。该项目采用模块化设计，具有清晰的架构和良好的可扩展性。

## 目录结构

```
MathEditorByQt/
├── CMakeLists.txt          # CMake构建配置文件
├── main.cpp               # 程序入口点
├── include/               # 头文件目录
│   ├── core/             # 核心数据模型
│   │   ├── Document.h
│   │   ├── Format.h
│   │   ├── Paragraph.h
│   │   ├── Run.h
│   │   └── Selection.h
│   ├── view/             # 用户界面层
│   │   ├── Cursor.h
│   │   ├── DocumentView.h
│   │   └── TextEditorWidget.h
│   ├── controller/       # 控制逻辑层
│   │   ├── DocumentController.h
│   │   ├── InputController.h
│   │   └── SelectionController.h
│   └── io/               # 输入输出模块
│       ├── DocumentReader.h
│       └── DocumentWriter.h
└── src/                   # 源代码目录
    ├── core/
    ├── view/
    ├── controller/
    └── io/
```

## 项目概述

MathEditorByQt 是一个现代化的数学公式编辑器项目，使用C++和Qt框架开发。该项目的目标是创建一个功能丰富、易于使用的数学文档编辑环境，支持复杂的数学公式的输入、编辑和显示。

该项目具有以下特点：

- **模块化设计**：采用清晰的模块划分，便于维护和扩展
- **现代C++**：使用C++17标准，充分利用现代C++特性
- **Qt框架**：基于Qt 6.x框架（兼容Qt 5.x），利用其强大的GUI功能
- **MVC架构**：采用经典的MVC架构模式，确保代码结构清晰
- **可扩展性**：设计灵活，便于添加新的功能模块

## 架构设计

MathEditorByQt采用了经典的MVC（Model-View-Controller）架构模式，各层职责明确：

### 模型层（Core Module）

模型层负责数据的存储和基本业务逻辑，包括以下组件：

- **Document（文档类）**：代表整个文档，包含多个段落，提供段落级别的操作
- **Paragraph（段落类）**：代表文档中的一个段落，由多个Run组成
- **Run（文本片段类）**：代表具有相同格式的一段连续文本
- **Format（格式类）**：定义文本的显示格式（字体、颜色、样式等）
- **Selection（选择类）**：表示文档中的选择区域，包含位置信息

模型层形成了一个典型的文档对象模型（DOM），其中Document包含多个Paragraph，每个Paragraph包含多个Run，每个Run有自己的Format。

### 视图层（View Module）

视图层负责用户界面的呈现和交互，包括以下组件：

- **TextEditorWidget（文本编辑器部件）**：主编辑器控件，作为中央部件嵌入到主窗口中，管理文档视图和其他编辑操作
- **DocumentView（文档视图）**：使用QGraphicsView实现的文档显示区域，负责渲染文档内容和处理用户交互
- **Cursor（光标）**：显示在文档中的可闪烁光标，指示当前编辑位置

视图层采用了Qt的Graphics View框架，使用QGraphicsScene和QGraphicsItem来高效地渲染文档内容。

### 控制器层（Controller Module）

控制器层负责处理用户输入和业务逻辑，包括以下组件：

- **DocumentController（文档控制器）**：负责文档内容的修改操作，如插入、删除、替换文本和段落操作
- **SelectionController（选择控制器）**：管理文档中的选择状态，提供选择操作和扩展功能
- **InputController（输入控制器）**：处理用户的键盘和鼠标输入，协调文档控制器和选择控制器的工作

控制器模块实现了编辑器的核心交互逻辑，将用户输入转换为对文档模型的操作。

### 输入输出层（IO Module）

输入输出层负责文档的读取和保存，包括以下组件：

- **DocumentReader（文档读取器）**：负责从文件或文本流中读取文档数据并还原为Document对象
- **DocumentWriter（文档写入器）**：负责将Document对象序列化为文件或文本流格式

该模块提供了文档的持久化功能，允许用户保存和加载文档。

### 设计模式应用

1. **组合模式（Composite Pattern）**：Document-Paragraph-Run的层级结构
2. **观察者模式（Observer Pattern）**：Qt信号槽机制
3. **策略模式（Strategy Pattern）**：不同类型的格式应用
4. **命令模式（Command Pattern）**：编辑操作的实现基础

## 技术栈与构建说明

### 技术栈

- **编程语言**：C++17
- **GUI框架**：Qt 6.x（兼容Qt 5.x）
- **构建系统**：CMake
- **图形渲染**：Qt Graphics View Framework
- **操作系统支持**：跨平台（Windows, Linux, macOS）

### 构建说明

1. **前置条件**
   - CMake 3.16 或更高版本
   - 支持C++17的编译器（如GCC 7+, Clang 5+, MSVC 2017+）
   - Qt 6.x 开发库（推荐Qt 6.2或更高版本，也兼容Qt 5.x）

2. **构建步骤**

   ```bash
   # 克隆或下载项目
   git clone <repository-url>
   cd MathEditorByQt

   # 创建构建目录
   mkdir build
   cd build

   # 配置项目
   cmake ..

   # 构建项目
   cmake --build .
   ```

3. **Windows平台构建**

   ```powershell
   # 使用PowerShell
   mkdir build
   cd build
   cmake .. -G "Visual Studio 16 2019"  # 或其他可用的生成器
   cmake --build . --config Release
   ```

4. **部署Qt依赖**

   构建完成后，若要在没有安装Qt的机器上运行，需要部署Qt运行时库：

   ```bash
   # Linux/macOS
   linuxdeployqt MathEditorByQt -appimage

   # Windows
   windeployqt MathEditorByQt.exe
   ```

## 使用说明

### 程序入口点

程序从`main.cpp`开始执行，创建了一个带有菜单栏的主窗口，包含文件操作（新建、打开、保存）和编辑操作（撤销、重做、剪切、复制、粘贴、全选）等功能。

### 功能特性

- **文档编辑**：支持多段落文本编辑
- **格式设置**：支持字体、颜色、样式等格式设置
- **选择操作**：支持文本选择、全选等功能
- **用户界面**：提供直观的菜单栏和编辑区域

### 示例用法

程序启动后，用户可以在编辑区域输入文本，使用菜单栏进行各种操作，如新建文档、保存文档等。

## 扩展与定制

该项目具有良好的可扩展性，开发者可以根据需要添加以下功能：

- 数学公式渲染引擎
- LaTeX导入/导出功能
- 富文本编辑功能
- 插入图片和其他媒体
- 文档模板系统
- 协作编辑功能

## 许可证

[在此处添加许可证信息]