# MathEditorByQt 架构设计文档

## 目录
1. [概述](#概述)
2. [核心概念](#核心概念)
3. [架构设计](#架构设计)
4. [格式系统](#格式系统)
5. [样式系统](#样式系统)
6. [文档结构](#文档结构)
7. [格式合并优先级](#格式合并优先级)
8. [实现细节](#实现细节)

---

## 概述

MathEditorByQt 是一个基于Qt框架的富文本编辑器，采用现代富文本编辑器的经典架构设计。本文档详细说明了编辑器的核心架构、设计原则和实现细节。

### 设计目标
- 支持样式和直接格式的分离
- 支持格式继承和覆盖
- 支持段落格式和字符格式的分离
- 支持未设置属性状态（使用继承值）
- 提供清晰的API和易于扩展的架构

---

## 核心概念

### 1. 样式 (Style) vs 直接格式 (Direct Format)

这是富文本编辑器中最核心的两个概念：

| 特性 | 样式 (Style) | 直接格式 (Direct Format) |
|------|--------------|---------------------|
| **定义位置** | 样式管理器中，全局定义 | 直接应用到特定文本或段落 |
| **作用范围** | 可以影响多个文本片段/段落 | 只影响特定文本片段/段落 |
| **可重用性** | 高，可命名复用 | 低，针对特定内容 |
| **修改影响** | 修改样式，所有引用它的内容都改变 | 只影响当前内容 |
| **示例** | "标题1"、"正文"、"强调" | 选中几个字加粗、改颜色 |

### 2. 段落样式 vs 字符样式

样式分为两种类型：

| 类型 | 作用对象 | 包含内容 | 示例 |
|------|----------|----------|------|
| **段落样式** | 整个段落 | 对齐方式、行间距、段前段后、首行缩进、基础字符格式 | "标题1"、"正文"、"列表项" |
| **字符样式** | 文本片段 | 字体、字号、颜色、粗体、斜体、下划线 | "强调"、"代码"、"引用" |

### 3. 属性状态：未设置 vs 已设置

每个格式属性都有三种状态：

- **未设置 (Unset)**: 属性未定义，使用继承自父格式的值
- **设置为真 (SetTrue)**: 属性已设置为 true
- **设置为假 (SetFalse)**: 属性已设置为 false

这种设计使得格式合并更加灵活和精确。

---

## 架构设计

### 整体架构图

```
┌─────────────────────────────────────────────────────────────┐
│                        用户界面层 (UI)                        │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │ TextEditor   │  │ 工具栏       │  │ 状态栏       │   │
│  │ Widget       │  │ (ToolBar)    │  │ (StatusBar)  │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                      控制器层 (Controller)                    │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │ Document     │  │ Selection    │  │ Input        │   │
│  │ Controller   │  │ Controller   │  │ Controller   │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                       视图层 (View)                           │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │ DocumentView │  │ TextRunItem  │  │ MathObject   │   │
│  │              │  │              │  │ Item         │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                      数据模型层 (Model)                       │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │   Document   │  │  Paragraph   │  │   TextRun    │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
└─────────────────────────────────────────────────────────────┘
                            ↓
┌─────────────────────────────────────────────────────────────┐
│                      核心服务层 (Core)                        │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐   │
│  │ StyleManager │  │   Format     │  │   Style      │   │
│  │              │  │  (Character/  │  │              │   │
│  │              │  │  Paragraph)   │  │              │   │
│  └──────────────┘  └──────────────┘  └──────────────┘   │
└─────────────────────────────────────────────────────────────┘
```

---

## 格式系统

### CharacterFormat (字符格式)

字符格式用于控制文本的字符级显示属性。

#### 属性列表

| 属性 | 类型 | 未设置值 | 说明 |
|------|------|----------|------|
| fontFamily | QString | 空字符串 | 字体族名称 |
| fontSize | int | 0 | 字号（点） |
| color | QColor | 无效颜色 | 文本颜色 |
| bold | PropertyState | Unset | 粗体 |
| italic | PropertyState | Unset | 斜体 |
| underline | PropertyState | Unset | 下划线 |

#### API 示例

```cpp
CharacterFormat format;

// 设置属性
format.setFontFamily("Microsoft YaHei");
format.setFontSize(12);
format.setColor(Qt::black);
format.setBold(true);

// 检查属性是否设置
if (format.hasFontFamily()) {
    QString family = format.fontFamily();
}

// 清除属性（恢复为未设置状态）
format.clearBold();

// 合并两个格式
CharacterFormat result = CharacterFormat::merge(baseFormat, overrideFormat);
```

### ParagraphFormat (段落格式)

段落格式用于控制段落的显示属性。

#### 属性列表

| 属性 | 类型 | 未设置值 | 说明 |
|------|------|----------|------|
| alignment | Alignment | Left | 对齐方式（左/中/右/两端） |
| lineSpacing | qreal | 0.0 | 行间距倍数 |
| spaceBefore | int | 0 | 段前间距（像素） |
| spaceAfter | int | 0 | 段后间距（像素） |
| firstLineIndent | int | 0 | 首行缩进（像素） |

#### API 示例

```cpp
ParagraphFormat format;

// 设置属性
format.setAlignment(Alignment::Center);
format.setLineSpacing(1.5);
format.setSpaceBefore(10);
format.setFirstLineIndent(20);

// 合并两个格式
ParagraphFormat result = ParagraphFormat::merge(baseFormat, overrideFormat);
```

### PropertyState (属性状态)

```cpp
enum class PropertyState {
    Unset,      // 未设置，使用继承值
    SetTrue,    // 已设置为true
    SetFalse    // 已设置为false
};
```

---

## 样式系统

### Style (样式)

样式是命名的格式集合，支持继承。

#### 样式类型

```cpp
enum class StyleType {
    Paragraph,  // 段落样式
    Character   // 字符样式
};
```

#### 样式组成

| 成员 | 类型 | 说明 |
|------|------|------|
| id | QString | 样式唯一标识符 |
| name | QString | 样式显示名称 |
| type | StyleType | 样式类型 |
| charFormat | CharacterFormat | 字符格式 |
| paraFormat | ParagraphFormat | 段落格式 |
| parentId | QString | 父样式ID（用于继承） |

#### 样式继承

样式支持基于父样式的继承：

```
默认样式
    ↓
父样式 (Parent Style)
    ↓
子样式 (Child Style) - 可以覆盖父样式的某些属性
```

#### API 示例

```cpp
// 创建段落样式
Style heading1(
    "heading1",
    "标题1",
    StyleType::Paragraph,
    CharacterFormat(),  // 字符格式
    ParagraphFormat(),   // 段落格式
    "normal"             // 父样式ID
);

// 设置格式
CharacterFormat charFmt;
charFmt.setFontFamily("Microsoft YaHei");
charFmt.setFontSize(18);
charFmt.setBold(true);
heading1.setCharacterFormat(charFmt);

ParagraphFormat paraFmt;
paraFmt.setSpaceBefore(20);
paraFmt.setSpaceAfter(10);
heading1.setParagraphFormat(paraFmt);
```

### StyleManager (样式管理器)

样式管理器采用单例模式，负责管理所有样式。

#### 主要功能

- 添加、删除、修改样式
- 根据ID查找样式
- 解析样式的最终格式（考虑继承）
- 通知样式变化

#### API 示例

```cpp
StyleManager *mgr = StyleManager::instance();

// 添加样式
mgr->addStyle(heading1);

// 获取样式
const Style *style = mgr->getStyle("heading1");

// 解析样式（考虑继承）
CharacterFormat charFmt = mgr->resolveCharacterFormat("heading1");
ParagraphFormat paraFmt = mgr->resolveParagraphFormat("heading1");
```

---

## 文档结构

### Document (文档)

文档是整个编辑器的根对象，包含多个段落。

```
Document
├── Paragraph 0
│   ├── styleId (段落样式ID)
│   ├── directParaFormat (段落直接格式)
│   └── 内容项列表
│       ├── TextRun 0
│       │   ├── styleId (字符样式ID)
│       │   ├── directCharFormat (字符直接格式)
│       │   └── text (文本内容)
│       ├── MathObject 1
│       └── TextRun 2
├── Paragraph 1
└── Paragraph 2
```

### Paragraph (段落)

段落是文档的基本组织单元。

| 成员 | 类型 | 说明 |
|------|------|------|
| styleId | QString | 段落样式ID |
| directParaFormat | ParagraphFormat | 段落直接格式 |
| items | QList\<Item\> | 内容项列表（TextRun或MathObject） |

### TextRun (文本片段)

文本片段是具有统一格式的文本单元。

| 成员 | 类型 | 说明 |
|------|------|------|
| text | QString | 文本内容 |
| styleId | QString | 字符样式ID |
| directCharFormat | CharacterFormat | 字符直接格式 |

---

## 格式合并优先级

### 字符格式合并优先级（从低到高）

```
1. 默认字符格式
    ↓
2. 文档默认字符样式
    ↓
3. 段落样式的字符格式
    ↓
4. 段落直接格式的字符部分
    ↓
5. 字符样式
    ↓
6. 字符直接格式 (最高优先级)
```

### 段落格式合并优先级（从低到高）

```
1. 默认段落格式
    ↓
2. 文档默认段落样式
    ↓
3. 段落样式
    ↓
4. 段落直接格式 (最高优先级)
```

### 合并算法

```cpp
// 合并两个格式：override的已设置属性会覆盖base的对应属性
CharacterFormat CharacterFormat::merge(const CharacterFormat &base, const CharacterFormat &override) {
    CharacterFormat result = base;
    
    if (override.hasFontFamily()) {
        result.setFontFamily(override.fontFamily());
    }
    if (override.hasFontSize()) {
        result.setFontSize(override.fontSize());
    }
    // ... 其他属性
    
    return result;
}
```

---

## 实现细节

### 文件结构

```
MathEditorByQt/
├── include/
│   └── core/
│       ├── PropertyState.h      # 属性状态枚举
│       ├── CharacterFormat.h    # 字符格式类
│       ├── ParagraphFormat.h    # 段落格式类
│       ├── Style.h              # 样式类
│       ├── StyleManager.h       # 样式管理器类
│       ├── Format.h             # (旧版，向后兼容)
│       ├── TextRun.h            # 文本片段类
│       ├── Paragraph.h          # 段落类
│       ├── Document.h           # 文档类
│       ├── Position.h           # 位置类
│       └── Selection.h          # 选择类
├── src/
│   └── core/
│       ├── CharacterFormat.cpp
│       ├── ParagraphFormat.cpp
│       ├── Style.cpp
│       ├── StyleManager.cpp
│       ├── TextRun.cpp
│       ├── Paragraph.cpp
│       └── Document.cpp
└── DESIGN.md                    # 本文档
```

### 关键类依赖关系

```
CharacterFormat ─┐
                  ├──> Style
ParagraphFormat ─┘
                  │
StyleManager ─────┘
                  │
Style ────────────┼──> TextRun
                  │
Document ─────────┼──> Paragraph
                  │
Selection ────────┘
```

### 使用示例

#### 1. 创建样式

```cpp
// 创建默认样式
Style normalStyle(
    "normal",
    "正文",
    StyleType::Paragraph
);

CharacterFormat normalCharFmt;
normalCharFmt.setFontFamily("Microsoft YaHei");
normalCharFmt.setFontSize(12);
normalCharFmt.setColor(Qt::black);
normalStyle.setCharacterFormat(normalCharFmt);

ParagraphFormat normalParaFmt;
normalParaFmt.setAlignment(Alignment::Left);
normalParaFmt.setLineSpacing(1.0);
normalStyle.setParagraphFormat(normalParaFmt);

// 添加到样式管理器
StyleManager::instance()->addStyle(normalStyle);

// 创建标题样式（继承自正文）
Style heading1Style(
    "heading1",
    "标题1",
    StyleType::Paragraph,
    CharacterFormat(),
    ParagraphFormat(),
    "normal"  // 父样式ID
);

CharacterFormat heading1CharFmt;
heading1CharFmt.setFontSize(18);
heading1CharFmt.setBold(true);
heading1Style.setCharacterFormat(heading1CharFmt);

ParagraphFormat heading1ParaFmt;
heading1ParaFmt.setSpaceBefore(20);
heading1ParaFmt.setSpaceAfter(10);
heading1Style.setParagraphFormat(heading1ParaFmt);

StyleManager::instance()->addStyle(heading1Style);
```

#### 2. 创建文档

```cpp
Document *doc = new Document();

// 创建第一个段落（标题）
Paragraph headingPara;
headingPara.setStyleId("heading1");

TextRun headingText("欢迎使用数学编辑器！");
headingPara.appendTextRun(headingText);

doc->addParagraph(headingPara);

// 创建第二个段落（正文）
Paragraph bodyPara;
bodyPara.setStyleId("normal");

TextRun bodyText1("这是一个基于Qt的富文本编辑器。");
bodyPara.appendTextRun(bodyText1);

TextRun bodyText2("支持样式和直接格式。");
CharacterFormat emphasisFormat;
emphasisFormat.setItalic(true);
emphasisFormat.setColor(Qt::blue);
bodyText2.setDirectCharFormat(emphasisFormat);
bodyPara.appendTextRun(bodyText2);

doc->addParagraph(bodyPara);
```

#### 3. 应用直接格式

```cpp
// 选中一段文本，应用直接格式
Selection selection(startPos, endPos);
CharacterFormat newFormat;
newFormat.setBold(true);
newFormat.setColor(Qt::red);

documentController->applyDirectFormat(selection, newFormat);
```

---

## 设计优势

### 1. 清晰的分离
- 段落格式和字符格式分离
- 样式和直接格式分离
- 数据模型和视图分离

### 2. 灵活的继承
- 样式支持继承，避免重复定义
- 格式合并时只覆盖已设置的属性
- 未设置的属性使用继承值

### 3. 易于扩展
- 新增格式属性只需修改相应的格式类
- 新增样式类型只需扩展StyleType枚举
- 清晰的API设计便于二次开发

### 4. 性能优化
- 格式合并时只处理必要的属性
- 样式管理器使用单例模式避免重复创建
- 延迟计算最终格式

---

## 总结

MathEditorByQt采用了现代富文本编辑器的经典架构设计，具有以下特点：

1. **分离关注点**：格式、样式、文档、视图各司其职
2. **支持继承**：样式继承和格式合并使得系统灵活强大
3. **未设置状态**：每个属性都有未设置状态，使用继承值
4. **清晰API**：提供直观、易于使用的编程接口
5. **易于扩展**：模块化设计便于功能扩展和维护

这种设计参考了Microsoft Word、LibreOffice Writer等成熟富文本编辑器的架构，同时保持了代码的简洁性和可维护性。
