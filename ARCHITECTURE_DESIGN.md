# MathEditorByQt 总体设计文档
=====================================

## 1. 项目概述
---------------

### 1.1 项目目标
开发一个基于 Qt 的数学公式编辑器，支持：
- 富文本编辑
- 数学公式插入
- 样式系统管理
- 灵活的布局系统

### 1.2 技术栈
- **语言**: C++17
- **GUI框架**: Qt (Qt5/Qt6
- **构建系统**: CMake

---

## 2. 系统架构
--------------

### 2.1 整体架构图

```
┌──────────────────────────────────────────────────────────────────────────┐
│                          View Layer (视图层)                          │
├──────────────────────────────────────────────────────────────────────────┤
│  TextEditorWidget  │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │ DocumentView                                      │   │
│  │  ┌──────────────────────────────────────────┐   │   │
│  │  │ ParagraphLayout (布局缓存)        │   │   │
│  │  │  ┌──────────────────────────────────┐   │   │   │
│  │  │  │ Line (行模型)              │   │   │   │
│  │  │  │ LineSegment (行片段)         │   │   │   │
│  │  │  └──────────────────────────────────┘   │   │   │
│  │  └──────────────────────────────────────────┘   │   │
│  │  Cursor                                  │   │
│  │  TextRunItem, MathObjectItem         │   │
│  └─────────────────────────────────────────────────────────┘   │
├──────────────────────────────────────────────────────────────────────────┤
│                       Controller Layer (控制器层)                      │
├──────────────────────────────────────────────────────────────────────────┤
│  DocumentController  │  InputController  │  SelectionController  │
├──────────────────────────────────────────────────────────────────────────┤
│                         Core Layer (核心层)                          │
├──────────────────────────────────────────────────────────────────────────┤
│  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐   │
│  │    Document      │  │   Paragraph     │  │    TextRun      │   │
│  └──────────────────┘  └──────────────────┘  └──────────────────┘   │
│  ┌──────────────────┐  ┌──────────────────┐  ┌──────────────────┐   │
│  │    MathObject   │  │   Selection     │  │    Character    │   │
│  └──────────────────┘  └──────────────────┘  └──────────────────┘   │
│  ┌──────────────────┐  ┌──────────────────┐                         │
│  │    Style        │  │  StyleManager   │                         │
│  └──────────────────┘  └──────────────────┘                         │
├──────────────────────────────────────────────────────────────────────────┤
│                        Util Layer (工具层)                          │
├──────────────────────────────────────────────────────────────────────────┤
│  PositionUtils  │  ParagraphUtils                                  │
├──────────────────────────────────────────────────────────────────────────┤
│                         IO Layer (IO层)                            │
├──────────────────────────────────────────────────────────────────────────┤
│  DocumentReader  │  DocumentWriter                                  │
└──────────────────────────────────────────────────────────────────────────┘
```

---

## 3. 核心模块详解
------------------

### 3.1 核心层 (Core Layer)

#### 3.1.1 PropertyState (属性状态)
**文件**: `include/core/PropertyState.h`

**功能**: 定义格式属性的三种状态

**枚举**:
```cpp
enum class PropertyState {
    Unset,     // 属性未设置，使用继承值
    SetTrue,   // 属性已设置为true
    SetFalse   // 属性已设置为false
};
```

---

#### 3.1.2 CharacterFormat (字符格式)
**文件**: `include/core/CharacterFormat.h`, `src/core/CharacterFormat.cpp`

**功能**: 定义文本的字符显示属性，支持未设置状态

**属性**:
- `fontFamily` - 字体族
- `fontSize` - 字号
- `color` - 颜色
- `boldState` - 粗体状态
- `italicState` - 斜体状态
- `underlineState` - 下划线状态

**主要方法**:
```cpp
// Getter/Setter
QString fontFamily() const;
void setFontFamily(const QString &family);
bool hasFontFamily() const;
void clearFontFamily();

int fontSize() const;
void setFontSize(int size);
bool hasFontSize() const;
void clearFontSize();

QColor color() const;
void setColor(const QColor &color);
bool hasColor() const;
void clearColor();

PropertyState boldState() const;
void setBold(bool bold);
bool hasBold() const;
void clearBold();

PropertyState italicState() const;
void setItalic(bool italic);
bool hasItalic() const;
void clearItalic();

PropertyState underlineState() const;
void setUnderline(bool underline);
bool hasUnderline() const;
void clearUnderline();

// 工具方法
static CharacterFormat merge(const CharacterFormat &base, const CharacterFormat &override);
QFont toFont(const QFont &defaultFont = QFont()) const;
```

---

#### 3.1.3 ParagraphFormat (段落格式)
**文件**: `include/core/ParagraphFormat.h`, `src/core/ParagraphFormat.cpp`

**功能**: 定义段落的格式属性

**属性**:
- `alignment` - 对齐方式
- `lineSpacing` - 行间距
- `spaceBefore` - 段前间距
- `spaceAfter` - 段后间距
- `indent` - 缩进

**主要方法**:
```cpp
// Getter/Setter 方法（与 CharacterFormat 类似）
Alignment alignment() const;
void setAlignment(Alignment align);
bool hasAlignment() const;
void clearAlignment();

qreal lineSpacing() const;
void setLineSpacing(qreal spacing);
bool hasLineSpacing() const;
void clearLineSpacing();

qreal spaceBefore() const;
void setSpaceBefore(qreal space);
bool hasSpaceBefore() const;
void clearSpaceBefore();

qreal spaceAfter() const;
void setSpaceAfter(qreal space);
bool hasSpaceAfter() const;
void clearSpaceAfter();

qreal indent() const;
void setIndent(qreal indent);
bool hasIndent() const;
void clearIndent();

static ParagraphFormat merge(const ParagraphFormat &base, const ParagraphFormat &override);
```

---

#### 3.1.4 TextRun (文本片段)
**文件**: `include/core/TextRun.h`, `src/core/TextRun.cpp`

**功能**: 表示具有统一格式的文本片段，是文档内容的基本单元

**属性**:
- `text` - 文本内容
- `styleId` - 样式ID
- `directFormat` - 直接字符格式覆盖
- `hasDirectFormat` - 是否有直接格式

**主要方法**:
```cpp
// 构造函数
TextRun();
TextRun(const QString &text, const CharacterFormat &charFormat = CharacterFormat());
TextRun(const QString &text, const QString &styleId, const CharacterFormat &directCharFormat = CharacterFormat());

// 文本操作
QString text() const;
void setText(const QString &text);
void insert(int position, const QString &text);
void remove(int position, int length);
int length() const;

// 样式管理
QString styleId() const;
void setStyleId(const QString &styleId);

// 直接格式
bool hasDirectFormat() const;
CharacterFormat directFormat() const;
void setDirectFormat(const CharacterFormat &format);
void clearDirectFormat();

// 最终格式
CharacterFormat effectiveFormat(StyleManager *styleMgr) const;
```

**与其他模块关系**:
- 被 Paragraph 包含
- 使用 CharacterFormat 进行格式化
- 通过 StyleManager 获取最终格式

---

#### 3.1.5 MathObject (数学对象)
**文件**: `include/core/MathObject.h`, `src/core/MathObject.cpp`

**功能**: 表示文档中的数学公式对象

**类型枚举**:
```cpp
enum Type {
    Placeholder,  // 占位符
    Fraction,     // 分数
    Radical,      // 根式
    Script,       // 上下标
    Matrix        // 矩阵
};
```

**主要方法**:
```cpp
MathObject(Type type = Placeholder);
Type type() const;
void setLatex(const QString &latex);
QString latex() const;
```

---

#### 3.1.6 Paragraph (段落)
**文件**: `include/core/Paragraph.h`, `src/core/Paragraph.cpp`

**功能**: 代表文档中的一个段落，由多个内容项(Item)组成

**内容项类型**:
```cpp
enum ItemType {
    TextRunItem,    // 文本片段项
    MathObjectItem  // 数学对象项
};
```

**内容项结构**:
```cpp
struct Item {
    ItemType type;      // 项的类型
    QVariant data;      // 项的数据：TextRun 或 MathObject
};
```

**主要方法**:
```cpp
// 构造函数
Paragraph();

// 添加内容
void appendText(const QString &text, const CharacterFormat &charFormat);
void appendText(const QString &text, const QString &styleId, const CharacterFormat &directCharFormat = CharacterFormat());
void appendMathObject(const MathObject &obj);

// 项管理
int itemCount() const;
const Item &itemAt(int index) const;
Item &itemAt(int index);
void insertItem(int index, const Item &item);
void replaceItems(int index, int count, const QList<Item> &newItems);

// 工具方法
QString plainText() const;

// 段落样式
QString styleId() const;
void setStyleId(const QString &styleId);

bool hasDirectFormat() const;
ParagraphFormat directFormat() const;
void setDirectFormat(const ParagraphFormat &format);
void clearDirectFormat();

CharacterFormat effectiveCharacterFormat(StyleManager *styleMgr) const;
ParagraphFormat effectiveParagraphFormat(StyleManager *styleMgr) const;
```

**与其他模块关系**:
- 被 Document 包含
- 包含 TextRun 和 MathObject
- 使用 StyleManager 获取最终格式

---

#### 3.1.7 Document (文档)
**文件**: `include/core/Document.h`, `src/core/Document.cpp`

**功能**: 文档类是整个文档系统的根节点，负责管理文档中的所有段落

**主要方法**:
```cpp
Document();

// 段落管理
void addParagraph(const Paragraph &paragraph);
void insertParagraph(int index, const Paragraph &paragraph);
void removeParagraph(int index);

Paragraph paragraph(int index) const;
Paragraph &paragraph(int index);
int paragraphCount() const;
void clear();
```

**与其他模块关系**:
- 包含多个 Paragraph
- 是文档的根节点

---

#### 3.1.8 Position & Selection (位置和选择)
**文件**: `include/core/Selection.h`, `src/core/Selection.cpp`

**Position 结构**:
```cpp
struct Position {
    int paragraphIndex;  // 段落索引
    int itemIndex;       // 内容项索引
    int offset;         // 项内偏移
};
```

**Position 比较运算符**:
```cpp
bool operator==(const Position &a, const Position &b);
bool operator!=(const Position &a, const Position &b);
bool operator<(const Position &a, const Position &b);
bool operator<=(const Position &a, const Position &b);
bool operator>(const Position &a, const Position &b);
bool operator>=(const Position &a, const Position &b);
```

**Selection 类主要方法**:
```cpp
Selection();
Selection(const Position &start, const Position &end);

Position start() const;
void setStart(const Position &pos);
Position end() const;
void setEnd(const Position &pos);

Position normalizedStart() const;
Position normalizedEnd() const;

bool isEmpty() const;
void extend(const Position &pos);
void collapseToStart();
void collapseToEnd();

bool isValid() const;
bool contains(const Position &pos) const;
```

---

#### 3.1.9 Style (样式)
**文件**: `include/core/Style.h`, `src/core/Style.cpp`

**功能**: 定义文档中的样式，支持继承

**样式类型**:
```cpp
enum class StyleType {
    Paragraph,  // 段落样式
    Character   // 字符样式
};
```

**样式分类**:
```cpp
enum class StyleCategory {
    All,
    BuiltIn,
    Custom,
    Paragraph,
    Character,
    Heading,
    List,
    Other
};
```

**主要方法**:
```cpp
// 构造函数
Style();
Style(const QString &id, const QString &name, StyleType type);
Style(const QString &id, const QString &name, StyleType type,
      const CharacterFormat &charFormat, const ParagraphFormat &paraFormat,
      const QString &parentId = QString());

// 属性访问
QString id() const;
void setId(const QString &id);
QString name() const;
void setName(const QString &name);
StyleType type() const;
void setType(StyleType type);

CharacterFormat charFormat() const;
void setCharFormat(const CharacterFormat &format);
ParagraphFormat paraFormat() const;
void setParaFormat(const ParagraphFormat &format);

QString parentId() const;
void setParentId(const QString &parentId);

// 新增属性
StyleCategory category() const;
void setCategory(StyleCategory category);
QString description() const;
void setDescription(const QString &description);
bool isBuiltIn() const;
void setBuiltIn(bool builtIn);
bool isHidden() const;
void setHidden(bool hidden);
int priority() const;
void setPriority(int priority);
QString nextStyleId() const;
void setNextStyleId(const QString &nextStyleId);

bool canDelete() const;
bool canModify() const;
Style clone() const;
```

---

#### 3.1.10 StyleManager (样式管理器)
**文件**: `include/core/StyleManager.h`, `src/core/StyleManager.cpp`

**功能**: 单例模式，管理所有样式，提供样式查找、解析继承、信号通知等

**主要方法**:
```cpp
// 单例访问
static StyleManager* instance();

// 样式管理
bool addStyle(const Style &style);
bool removeStyle(const QString &styleId);
bool hasStyle(const QString &styleId) const;
Style style(const QString &styleId) const;
QList<Style> allStyles() const;
int styleCount() const;
void clear();

// 继承解析
CharacterFormat resolveCharacterFormat(const QString &styleId) const;
ParagraphFormat resolveParagraphFormat(const QString &styleId) const;

// 默认样式
void initializeDefaultStyles();
void resetToDefaults();
bool isDefaultStyle(const QString &styleId) const;

// 样式查询
QList<Style> stylesByCategory(StyleCategory category) const;
QList<Style> stylesByType(StyleType type) const;
QList<Style> searchStyles(const QString &keyword) const;
QList<Style> availableParentStyles(const QString &currentStyleId) const;

// 验证
bool isValidStyleId(const QString &styleId) const;
QString generateUniqueStyleId(const QString &baseName = "CustomStyle") const;
bool hasInheritanceCycle(const QString &styleId) const;

// 持久化
bool loadFromJson(const QJsonObject &json);
QJsonObject saveToJson() const;
bool loadFromFile(const QString &filePath);
bool saveToFile(const QString &filePath) const;

// 信号
void styleAdded(const QString &styleId);
void styleRemoved(const QString &styleId);
void styleUpdated(const QString &styleId);
void styleRenamed(const QString &oldId, const QString &newId);
```

---

### 3.2 视图层 (View Layer)

#### 3.2.1 Line (行)
**文件**: `include/view/Line.h`, `src/view/Line.cpp`

**功能**: 表示文档中的一行文本，包含该行的布局信息

**LineSegment 结构**:
```cpp
struct LineSegment {
    int itemIndex;      // 对应的段落项索引
    int offsetStart;    // 在项中的起始偏移
    int offsetEnd;      // 在项中的结束偏移
    qreal x;           // 水平位置
    qreal width;       // 宽度
    qreal ascent;       // 上行高度
    qreal descent;      // 下行高度
};
```

**主要方法**:
```cpp
Line();
Line(int paragraphIndex, int lineIndex);

int paragraphIndex() const;
int lineIndex() const;

void setRect(const QRectF &rect);
QRectF rect() const;

void setBaseline(qreal baseline);
qreal baseline() const;

void addSegment(const LineSegment &segment);
const QList<LineSegment> &segments() const;

Position startPosition() const;
void setStartPosition(const Position &pos);
Position endPosition() const;
void setEndPosition(const Position &pos);

bool containsPosition(const Position &pos) const;
bool findPositionAtX(qreal x, int &segmentIndex, int &offset) const;
void clear();
```

---

#### 3.2.2 ParagraphLayout (段落布局)
**文件**: `include/view/ParagraphLayout.h`, `src/view/ParagraphLayout.cpp`

**功能**: 负责计算段落的布局，将段落内容分成多行

**主要方法**:
```cpp
ParagraphLayout(QObject *parent = nullptr);

void setParagraph(const Paragraph *paragraph);
void setParagraphIndex(int index);
int paragraphIndex() const;

void setMaxWidth(qreal width);
qreal maxWidth() const;

void layout();
const QList<Line> &lines() const;
int lineCount() const;
const Line &line(int index) const;
qreal height() const;

int findLineForPosition(const Position &pos) const;
Position positionFromPoint(const QPointF &point) const;
QPointF pointFromPosition(const Position &pos) const;

void markDirty();
bool isDirty() const;
void clear();
```

---

#### 3.2.3 DocumentView (文档视图)
**文件**: `include/view/DocumentView.h`, `src/view/DocumentView.cpp`

**功能**: 提供文档的图形化显示和交互功能

**主要方法**:
```cpp
DocumentView(QWidget *parent = nullptr);

void setDocument(Document *document);
Document *document() const;

void setSelection(const Selection &selection);
Selection selection() const;

Cursor *cursor() const;

void updateLayout();
void ensureCursorVisible();

// 坐标转换
Position positionFromPoint(const QPointF &point) const;
QPointF pointFromPosition(const Position &pos) const;

// 事件处理
void mousePressEvent(QMouseEvent *event) override;
void mouseMoveEvent(QMouseEvent *event) override;
void mouseReleaseEvent(QMouseEvent *event) override;
void keyPressEvent(QKeyEvent *event) override;
void resizeEvent(QResizeEvent *event) override;

// 信号
void selectionChanged(const Selection &selection);
void mousePositionChanged(const QPointF &scenePos, const QPoint &viewPos);
```

**私有方法**:
```cpp
void updateInputMethod();
void clearGraphicsItems();
void rebuildScene();
ParagraphLayout *getOrCreateLayout(int paragraphIndex);
void updateAllLayouts();
void clearLayouts();
```

---

#### 3.2.4 TextRunItem & MathObjectItem
**文件**: 
- `include/view/TextRunItem.h`, `src/view/TextRunItem.cpp`
- `include/view/MathObjectItem.h`, `src/view/MathObjectItem.cpp`

**功能**: 文本和数学对象的图形表示

---

#### 3.2.5 Cursor (光标)
**文件**: `include/view/Cursor.h`, `src/view/Cursor.cpp`

**功能**: 文档光标显示和管理

---

#### 3.2.6 TextEditorWidget
**文件**: `include/view/TextEditorWidget.h`, `src/view/TextEditorWidget.cpp`

**功能**: 主编辑器窗口

---

### 3.3 控制器层 (Controller Layer)

#### 3.3.1 DocumentController
**文件**: `include/controller/DocumentController.h`, `src/controller/DocumentController.cpp`

**功能**: 文档内容的编辑操作，包括文本插入、替换、删除等

**主要方法**:
```cpp
DocumentController(QObject *parent);

void setDocument(Document *document);
Document *document() const;

void insertText(const Position &position, const QString &text);
void replaceText(const Selection &selection, const QString &text);
void deleteText(const Selection &selection);
void insertParagraph(int paragraphIndex);

void applyStyle(const Selection &selection, const QString &styleId);
void applyDirectFormat(const Selection &selection, const CharacterFormat &format);

CharacterFormat currentDirectFormat() const;
void setCurrentDirectFormat(const CharacterFormat &format);
```

---

#### 3.3.2 SelectionController
**文件**: `include/controller/SelectionController.h`, `src/controller/SelectionController.cpp`

**功能**: 选择区域管理

---

#### 3.3.3 InputController
**文件**: `include/controller/InputController.h`, `src/controller/InputController.cpp`

**功能**: 输入处理

---

### 3.4 工具层 (Util Layer)

#### 3.4.1 PositionUtils
**文件**: `include/util/PositionUtils.h`, `src/util/PositionUtils.cpp`

**功能**: 位置和字符信息计算

---

#### 3.4.2 ParagraphUtils
**文件**: `include/util/ParagraphUtils.h`, `src/util/ParagraphUtils.cpp`

**功能**: 段落操作工具

**主要方法**:
```cpp
void applyFormatToSelection(Paragraph &para, int itemStart, int offsetStart,
                         int itemEnd, int offsetEnd,
                         std::function<void(TextRun&)> formatOperation);

Paragraph::Item createTextItem(const TextRun &run);
void splitTextRun(const TextRun &run, int splitOffset,
                 TextRun &before, TextRun &after);
TextRun extractTextRunSubset(const TextRun &run, int start, int length);
```

---

### 3.5 IO层 (IO Layer)

#### 3.5.1 DocumentReader & DocumentWriter
**文件**: 
- `include/io/DocumentReader.h`, `src/io/DocumentReader.cpp`
- `include/io/DocumentWriter.h`, `src/io/DocumentWriter.cpp`

**功能**: 文档的读写操作

---

## 4. 模块关系图
-------------

### 4.1 数据模型关系

```
Document (文档)
    │
    ├─ contains N ──> Paragraph (段落)
    │                     │
    │                     ├─ contains N ──> TextRun (文本片段)
    │                     │                     │
    │                     │                     ├─ uses ──> CharacterFormat
    │                     │                     └─ uses ──> StyleManager
    │                     │
    │                     └─ contains N ──> MathObject (数学对象)
    │
    └─ uses ──> StyleManager (样式管理器)
                          │
                          └─ contains N ──> Style (样式)
                                          │
                                          ├─ has ──> CharacterFormat
                                          └─ has ──> ParagraphFormat
```

### 4.2 视图层关系

```
TextEditorWidget
    │
    └─ contains ──> DocumentView
                     │
                     ├─ contains ──> Cursor
                     │
                     ├─ manages ──> ParagraphLayout (缓存)
                     │               │
                     │               └─ contains N ──> Line
                     │                             │
                     │                             └─ contains N ──> LineSegment
                     │
                     └─ renders ──> TextRunItem, MathObjectItem
```

### 4.3 控制器与其他模块关系

```
DocumentController
    │
    ├─ controls ──> Document
    │
    ├─ uses ──> StyleManager
    │
    └─ uses ──> ParagraphUtils

SelectionController
    │
    └─ manages ──> Selection

InputController
    │
    └─ handles ──> user input
```

---

## 5. 数据流程
-----------

### 5.1 文本插入流程

```
用户输入
    │
    ├─> InputController 接收输入
    │
    ├─> DocumentController::insertText()
    │
    ├─> Document 修改数据
    │
    ├─> DocumentView::updateLayout()
    │
    ├─> ParagraphLayout 重新布局（如果需要）
    │
    └─> DocumentView 重新渲染
```

### 5.2 样式应用流程

```
用户选择样式
    │
    ├─> DocumentController::applyStyle()
    │
    ├─> ParagraphUtils::applyFormatToSelection()
    │
    ├─> TextRun 设置 styleId
    │
    ├─> DocumentView::rebuildScene()
    │
    └─> 重新渲染
```

### 5.3 布局计算流程

```
文档内容变化
    │
    ├─> ParagraphLayout::markDirty()
    │
    ├─> ParagraphLayout::layout()
    │   │
    │   ├─> 计算文本宽度
    │   ├─> 自动换行
    │   ├─> 创建 Line 对象
    │   └─> 计算坐标
    │
    └─> DocumentView::rebuildScene()
```

---

## 6. 关键设计决策
---------------

### 6.1 样式系统设计

**特点**:
- 样式继承机制
- 直接格式覆盖
- 支持段落和字符样式
- 样式分类（内置/自定义）
- JSON 持久化

**格式优先级**:
```
直接格式 > 样式格式 > 继承样式 > 默认样式
```

### 6.2 布局系统设计

**特点**:
- 折中方案：布局缓存放在视图层
- Line 模型支持自动换行
- ParagraphLayout 缓存布局结果
- 支持不同字号混排
- 响应式布局（视图宽度变化时重新布局）

### 6.3 位置表示

**三级索引**:
```
Position {
    paragraphIndex  // 段落索引
    itemIndex       // 项索引
    offset         // 项内偏移
}
```

---

## 7. 文件清单
-----------

```
MathEditorByQt/
├── CMakeLists.txt
├── main.cpp
├── STYLE_MANAGER_DESIGN.md
├── ARCHITECTURE_DESIGN.md (本文件)
│
├── include/
│   ├── core/
│   │   ├── PropertyState.h
│   │   ├── CharacterFormat.h
│   │   ├── ParagraphFormat.h
│   │   ├── TextRun.h
│   │   ├── MathObject.h
│   │   ├── Paragraph.h
│   │   ├── Document.h
│   │   ├── Selection.h
│   │   ├── Style.h
│   │   └── StyleManager.h
│   │
│   ├── view/
│   │   ├── Cursor.h
│   │   ├── TextRunItem.h
│   │   ├── MathObjectItem.h
│   │   ├── DocumentView.h
│   │   ├── TextEditorWidget.h
│   │   ├── Line.h
│   │   └── ParagraphLayout.h
│   │
│   ├── controller/
│   │   ├── DocumentController.h
│   │   ├── SelectionController.h
│   │   └── InputController.h
│   │
│   ├── util/
│   │   ├── PositionUtils.h
│   │   └── ParagraphUtils.h
│   │
│   └── io/
│       ├── DocumentReader.h
│       └── DocumentWriter.h
│
└── src/
    ├── core/
    │   ├── CharacterFormat.cpp
    │   ├── ParagraphFormat.cpp
    │   ├── TextRun.cpp
    │   ├── MathObject.cpp
    │   ├── Paragraph.cpp
    │   ├── Document.cpp
    │   ├── Selection.cpp
    │   ├── Style.cpp
    │   └── StyleManager.cpp
    │
    ├── view/
    │   ├── Cursor.cpp
    │   ├── TextRunItem.cpp
    │   ├── MathObjectItem.cpp
    │   ├── DocumentView.cpp
    │   ├── TextEditorWidget.cpp
    │   ├── Line.cpp
    │   └── ParagraphLayout.cpp
    │
    ├── controller/
    │   ├── DocumentController.cpp
    │   ├── SelectionController.cpp
    │   └── InputController.cpp
    │
    ├── util/
    │   ├── PositionUtils.cpp
    │   └── ParagraphUtils.cpp
    │
    └── io/
        ├── DocumentReader.cpp
        └── DocumentWriter.cpp
```

---

## 8. 扩展点
---------

### 8.1 可扩展的功能

1. **更多数学对象类型**: 在 MathObject 中添加新的类型
2. **更复杂的布局算法**: 改进 ParagraphLayout 的换行算法
3. **导出/导入格式**: 在 IO 层支持更多格式
4. **撤销/重做**: 在控制器层添加命令模式
5. **更多样式特性**: 在 StyleManager 中添加更多属性

---

## 9. 总结
---------

本设计文档详细描述了 MathEditorByQt 项目的整体架构：

- **清晰的分层架构**: 核心层、视图层、控制器层、工具层、IO层
- **完整的样式系统**: 支持继承、直接格式覆盖、持久化
- **灵活的布局系统**: Line 模型支持自动换行和响应式布局
- **模块化设计**: 各模块职责明确，易于维护和扩展

项目采用了现代 C++17 和 Qt 框架，具有良好的可扩展性和可维护性。
