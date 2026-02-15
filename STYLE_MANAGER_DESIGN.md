# 样式管理器 (StyleManager) 设计文档

## 目录
1. [概述](#概述)
2. [设计目标](#设计目标)
3. [核心概念](#核心概念)
4. [系统架构](#系统架构)
5. [详细设计](#详细设计)
6. [默认样式集](#默认样式集)
7. [API 参考](#api-参考)
8. [使用示例](#使用示例)

---

## 概述

样式管理器是富文本编辑器的核心组件之一，负责管理和组织所有样式定义。它采用单例模式，提供样式的存储、检索、继承解析和变更通知等功能。

本设计参考了 Microsoft Word、LibreOffice Writer 和 Google Docs 等成熟富文本编辑器的样式系统设计。

---

## 设计目标

### 1. 核心功能
- ✅ 支持段落样式和字符样式的分离管理
- ✅ 支持样式的继承链（单继承）
- ✅ 提供样式的增删改查操作
- ✅ 支持样式的持久化存储和加载
- ✅ 提供样式变更的信号通知机制
- ✅ 支持样式的导入和导出
- ✅ 提供默认样式集的初始化

### 2. 非功能性目标
- **高性能**：样式解析和格式合并应高效
- **线程安全**：考虑到未来可能的多线程需求
- **可扩展**：易于添加新的样式类型和属性
- **易用性**：提供简洁直观的 API

---

## 核心概念

### 1. 样式类型 (StyleType)

样式分为两种主要类型：

| 类型 | 说明 | 适用对象 |
|------|------|----------|
| **段落样式** | 控制整个段落的格式 | 段落 |
| **字符样式** | 控制文本片段的字符格式 | TextRun |

### 2. 样式继承 (Style Inheritance)

样式支持单继承，子样式继承父样式的所有属性，并可以覆盖其中的某些属性。

```
normal (默认样式)
  ├─ heading1 (继承自 normal，覆盖字号、粗体等)
  ├─ heading2 (继承自 normal，覆盖字号、粗体等)
  └─ body (继承自 normal)
      └─ body_indent (继承自 body，覆盖首行缩进)
```

**继承规则**：
- 子样式未设置的属性使用父样式的值
- 子样式已设置的属性覆盖父样式的值
- 支持多级继承（如 normal → body → body_indent）
- 循环继承检测（防止 A → B → A 这样的循环）

### 3. 样式分类 (StyleCategory)

为了便于在 UI 中展示和组织，样式可以分为不同的分类：

| 分类 | 说明 | 示例 |
|------|------|------|
| **BuiltIn** | 内置样式，不可删除 | normal, heading1, heading2 |
| **Custom** | 用户自定义样式 | MyStyle, MyHeading |
| **CharacterOnly** | 仅字符样式 | Emphasis, Code, Strong |
| **ParagraphOnly** | 仅段落样式 | Quote, ListItem |

### 4. 样式优先级

当多个样式同时应用时，优先级从低到高：

```
1. 内置默认样式
   ↓
2. 文档默认段落样式
   ↓
3. 段落样式（应用于整个段落）
   ↓
4. 段落直接格式
   ↓
5. 字符样式（应用于文本片段）
   ↓
6. 字符直接格式（最高优先级）
```

---

## 系统架构

### 整体架构图

```
┌─────────────────────────────────────────────────────────────┐
│                     StyleManager (单例)                      │
│  ┌───────────────────────────────────────────────────────┐  │
│  │              样式存储层 (Storage Layer)                │  │
│  │  - QMap<QString, Style> 样式映射                      │  │
│  │  - 默认样式初始化                                       │  │
│  └───────────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────────┐  │
│  │             样式解析层 (Resolution Layer)               │  │
│  │  - resolveCharacterFormat()                            │  │
│  │  - resolveParagraphFormat()                            │  │
│  │  - 继承链解析                                           │  │
│  └───────────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────────┐  │
│  │            样式管理层 (Management Layer)                 │  │
│  │  - addStyle() / updateStyle() / removeStyle()          │  │
│  │  - getStyle() / styleIds() / allStyles()               │  │
│  │  - loadStyles() / saveStyles()                         │  │
│  └───────────────────────────────────────────────────────┘  │
│  ┌───────────────────────────────────────────────────────┐  │
│  │             信号通知层 (Notification Layer)              │  │
│  │  - styleAdded() / styleRemoved() / styleUpdated()      │  │
│  │  - stylesChanged()                                       │  │
│  └───────────────────────────────────────────────────────┘  │
└─────────────────────────────────────────────────────────────┘
                           ↓
┌─────────────────────────────────────────────────────────────┐
│                      样式对象 (Style)                         │
│  - id, name, type, category                                 │
│  - charFormat, paraFormat                                   │
│  - parentId, hasParent()                                    │
└─────────────────────────────────────────────────────────────┘
```

---

## 详细设计

### 1. 样式类增强 (Style)

在现有 Style 类的基础上，增加以下功能：

#### 新增属性

| 属性 | 类型 | 说明 |
|------|------|------|
| **category** | StyleCategory | 样式分类 |
| **description** | QString | 样式描述 |
| **isBuiltIn** | bool | 是否为内置样式 |
| **isHidden** | bool | 是否在 UI 中隐藏 |
| **priority** | int | 显示优先级（用于排序） |
| **nextStyleId** | QString | 下一个段落的默认样式 |

#### 新增方法

```cpp
// 检查是否可以删除（内置样式不可删除）
bool canDelete() const;

// 检查是否可以修改（内置样式的某些属性不可修改）
bool canModify() const;

// 深拷贝
Style clone() const;

// 获取完整的继承链（从当前样式到根样式）
QList<QString> inheritanceChain() const;
```

### 2. 样式分类枚举 (StyleCategory)

```cpp
enum class StyleCategory {
    All,            // 所有样式
    BuiltIn,        // 内置样式
    Custom,         // 自定义样式
    Paragraph,      // 段落样式
    Character,      // 字符样式
    Heading,        // 标题样式
    List,           // 列表样式
    Other           // 其他样式
};
```

### 3. 样式管理器增强 (StyleManager)

#### 新增功能模块

##### 3.1 默认样式管理

```cpp
// 初始化默认样式集
void initializeDefaultStyles();

// 重置为默认样式
void resetToDefaults();

// 检查是否为默认样式ID
bool isDefaultStyle(const QString &styleId) const;
```

##### 3.2 样式持久化

```cpp
// 从 JSON 加载样式
bool loadFromJson(const QJsonObject &json);

// 保存为 JSON
QJsonObject saveToJson() const;

// 从文件加载
bool loadFromFile(const QString &filePath);

// 保存到文件
bool saveToFile(const QString &filePath) const;

// 导入样式（合并到现有样式）
bool importStyles(const QString &filePath);

// 导出样式
bool exportStyles(const QString &filePath, const QList<QString> &styleIds = QList<QString>());
```

##### 3.3 样式查询和过滤

```cpp
// 根据分类获取样式
QList<Style> stylesByCategory(StyleCategory category) const;

// 根据类型获取样式
QList<Style> stylesByType(StyleType type) const;

// 搜索样式（根据名称或ID）
QList<Style> searchStyles(const QString &keyword) const;

// 获取可用的父样式列表（排除自身和循环继承）
QList<Style> availableParentStyles(const QString &currentStyleId) const;
```

##### 3.4 样式验证

```cpp
// 验证样式ID是否有效
bool isValidStyleId(const QString &styleId) const;

// 生成唯一的样式ID
QString generateUniqueStyleId(const QString &baseName = "CustomStyle") const;

// 检查继承链是否有循环
bool hasInheritanceCycle(const QString &styleId) const;
```

##### 3.5 增强的信号

```cpp
signals:
    // 样式添加
    void styleAdded(const QString &styleId);
    
    // 样式删除
    void styleRemoved(const QString &styleId);
    
    // 样式更新
    void styleUpdated(const QString &styleId);
    
    // 样式列表变化
    void stylesChanged();
    
    // 样式被重命名
    void styleRenamed(const QString &oldId, const QString &newId);
```

---

## 默认样式集

### 段落样式

| ID | 名称 | 父样式 | 说明 |
|----|------|--------|------|
| `normal` | 正文 | - | 默认段落样式 |
| `heading1` | 标题 1 | normal | 一级标题 |
| `heading2` | 标题 2 | normal | 二级标题 |
| `heading3` | 标题 3 | normal | 三级标题 |
| `heading4` | 标题 4 | normal | 四级标题 |
| `heading5` | 标题 5 | normal | 五级标题 |
| `heading6` | 标题 6 | normal | 六级标题 |
| `quote` | 引用 | normal | 引用样式 |
| `code_block` | 代码块 | normal | 代码块样式 |
| `list_bullet` | 无序列表 | normal | 无序列表项 |
| `list_number` | 有序列表 | normal | 有序列表项 |

### 字符样式

| ID | 名称 | 说明 |
|----|------|------|
| `emphasis` | 强调 | 斜体 |
| `strong` | 加粗 | 粗体 |
| `code` | 代码 | 等宽字体 |
| `strikethrough` | 删除线 | 删除线 |
| `superscript` | 上标 | 上标 |
| `subscript` | 下标 | 下标 |
| `hyperlink` | 超链接 | 蓝色带下划线 |

### 默认样式配置

```cpp
// normal (正文)
CharacterFormat:
  - fontFamily: "Microsoft YaHei"
  - fontSize: 12
  - color: Qt::black
  - bold: Unset
  - italic: Unset
  - underline: Unset

ParagraphFormat:
  - alignment: Left
  - lineSpacing: 1.0
  - spaceBefore: 0
  - spaceAfter: 0
  - firstLineIndent: 0

// heading1 (标题 1)
CharacterFormat:
  - fontSize: 18
  - bold: SetTrue
  - color: Qt::darkBlue

ParagraphFormat:
  - spaceBefore: 24
  - spaceAfter: 12
  - nextStyleId: "normal"

// heading2 (标题 2)
CharacterFormat:
  - fontSize: 16
  - bold: SetTrue
  - color: Qt::darkBlue

ParagraphFormat:
  - spaceBefore: 18
  - spaceAfter: 9
  - nextStyleId: "normal"

// emphasis (强调)
CharacterFormat:
  - italic: SetTrue

// strong (加粗)
CharacterFormat:
  - bold: SetTrue

// code (代码)
CharacterFormat:
  - fontFamily: "Consolas"
  - color: Qt::darkRed
```

---

## API 参考

### StyleManager 类

#### 单例访问

```cpp
static StyleManager* instance();
```

#### 样式管理

```cpp
// 添加样式
bool addStyle(const Style &style);

// 更新样式
bool updateStyle(const Style &style);

// 删除样式
bool removeStyle(const QString &styleId);

// 获取样式
const Style* getStyle(const QString &styleId) const;

// 获取所有样式ID
QList<QString> styleIds() const;

// 获取所有样式
QList<Style> allStyles() const;
```

#### 样式解析

```cpp
// 解析字符格式（考虑继承）
CharacterFormat resolveCharacterFormat(const QString &styleId) const;

// 解析段落格式（考虑继承）
ParagraphFormat resolveParagraphFormat(const QString &styleId) const;

// 合并字符格式
static CharacterFormat mergeCharacterFormat(const CharacterFormat &base, const CharacterFormat &direct);

// 合并段落格式
static ParagraphFormat mergeParagraphFormat(const ParagraphFormat &base, const ParagraphFormat &direct);
```

#### 样式查询

```cpp
// 根据分类获取
QList<Style> stylesByCategory(StyleCategory category) const;

// 根据类型获取
QList<Style> stylesByType(StyleType type) const;

// 搜索样式
QList<Style> searchStyles(const QString &keyword) const;
```

#### 持久化

```cpp
// 初始化默认样式
void initializeDefaultStyles();

// 重置为默认
void resetToDefaults();

// 从 JSON 加载
bool loadFromJson(const QJsonObject &json);

// 保存为 JSON
QJsonObject saveToJson() const;

// 从文件加载
bool loadFromFile(const QString &filePath);

// 保存到文件
bool saveToFile(const QString &filePath) const;
```

---

## 使用示例

### 1. 初始化样式管理器

```cpp
// 获取单例
StyleManager *mgr = StyleManager::instance();

// 初始化默认样式
mgr->initializeDefaultStyles();
```

### 2. 创建自定义样式

```cpp
// 创建段落样式
Style myParagraphStyle(
    "my_paragraph",      // ID
    "我的段落样式",       // 名称
    StyleType::Paragraph, // 类型
    CharacterFormat(),    // 字符格式
    ParagraphFormat(),    // 段落格式
    "normal"              // 父样式
);

// 设置字符格式
CharacterFormat charFmt;
charFmt.setFontFamily("Microsoft YaHei");
charFmt.setFontSize(14);
charFmt.setColor(Qt::darkGray);
myParagraphStyle.setCharacterFormat(charFmt);

// 设置段落格式
ParagraphFormat paraFmt;
paraFmt.setAlignment(Alignment::Justify);
paraFmt.setLineSpacing(1.5);
paraFmt.setFirstLineIndent(28);
myParagraphStyle.setParagraphFormat(paraFmt);

// 添加到管理器
mgr->addStyle(myParagraphStyle);
```

### 3. 创建字符样式

```cpp
Style myCharStyle(
    "my_character",
    "我的字符样式",
    StyleType::Character
);

CharacterFormat charFmt;
charFmt.setBold(true);
charFmt.setItalic(true);
charFmt.setColor(Qt::blue);
charFmt.setUnderline(true);
myCharStyle.setCharacterFormat(charFmt);

mgr->addStyle(myCharStyle);
```

### 4. 使用样式

```cpp
// 获取样式
const Style *style = mgr->getStyle("heading1");
if (style) {
    qDebug() << "样式名称:" << style->name();
}

// 解析最终格式（考虑继承）
CharacterFormat resolvedCharFmt = mgr->resolveCharacterFormat("heading1");
ParagraphFormat resolvedParaFmt = mgr->resolveParagraphFormat("heading1");

// 应用到文本
TextRun run("这是标题");
run.setStyleId("heading1");
```

### 5. 应用直接格式覆盖

```cpp
// 基础格式（来自样式）
CharacterFormat baseFmt = mgr->resolveCharacterFormat("normal");

// 直接格式覆盖
CharacterFormat directFmt;
directFmt.setBold(true);
directFmt.setColor(Qt::red);

// 合并
CharacterFormat finalFmt = StyleManager::mergeCharacterFormat(baseFmt, directFmt);
```

### 6. 保存和加载样式

```cpp
// 保存到文件
mgr->saveToFile("styles.json");

// 从文件加载
mgr->loadFromFile("styles.json");

// 导出特定样式
QList<QString> exportIds;
exportIds << "heading1" << "heading2";
mgr->exportStyles("my_styles.json", exportIds);
```

### 7. 查询和过滤

```cpp
// 获取所有段落样式
QList<Style> paragraphStyles = mgr->stylesByType(StyleType::Paragraph);

// 获取所有标题样式
QList<Style> headingStyles = mgr->stylesByCategory(StyleCategory::Heading);

// 搜索样式
QList<Style> searchResults = mgr->searchStyles("标题");

// 获取可用的父样式
QList<Style> availableParents = mgr->availableParentStyles("my_style");
```

---

## 实现计划

### Phase 1: 基础功能增强
- [ ] 增强 Style 类（新增属性和方法）
- [ ] 添加 StyleCategory 枚举
- [ ] 更新 StyleManager 基础 API

### Phase 2: 默认样式
- [ ] 定义默认样式配置
- [ ] 实现 initializeDefaultStyles()
- [ ] 实现 resetToDefaults()

### Phase 3: 持久化
- [ ] 实现 JSON 序列化/反序列化
- [ ] 实现文件加载/保存
- [ ] 实现导入/导出

### Phase 4: 查询和验证
- [ ] 实现样式过滤和搜索
- [ ] 实现继承循环检测
- [ ] 实现样式验证

### Phase 5: UI 集成
- [ ] 样式面板设计
- [ ] 样式选择器
- [ ] 样式编辑器

---

## 总结

本样式管理器设计提供了完整的富文本编辑器样式系统解决方案，具有以下特点：

1. **完整的功能**：样式管理、继承、持久化、查询等
2. **清晰的架构**：分层设计，职责明确
3. **易于使用**：提供简洁直观的 API
4. **可扩展**：模块化设计，易于添加新功能
5. **参考成熟方案**：借鉴 Word、LibreOffice 等的设计

该设计将为 MathEditorByQt 提供强大而灵活的样式管理功能。
