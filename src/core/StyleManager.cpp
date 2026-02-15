// ============================================================================
// StyleManager.cpp
// 样式管理器实现文件
// 管理所有样式，提供样式查找、解析继承、信号通知等
// ============================================================================

#include "core/StyleManager.h"
#include <QFont>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <algorithm>

StyleManager* StyleManager::m_instance = nullptr;

StyleManager* StyleManager::instance() {
    if (!m_instance) {
        m_instance = new StyleManager();
    }
    return m_instance;
}

StyleManager::StyleManager(QObject *parent) : QObject(parent) {
    initializeDefaultStyles();
}

StyleManager::~StyleManager() {}

void StyleManager::initializeDefaultStyles() {
    m_styles.clear();
    m_defaultStyleIds.clear();

    // ==================== 段落样式 ====================

    // normal (正文)
    {
        CharacterFormat charFmt;
        charFmt.setFontFamily("Microsoft YaHei");
        charFmt.setFontSize(12);
        charFmt.setColor(Qt::black);

        ParagraphFormat paraFmt;
        paraFmt.setAlignment(Alignment::Left);
        paraFmt.setLineSpacing(1.0);
        paraFmt.setSpaceBefore(0);
        paraFmt.setSpaceAfter(0);
        paraFmt.setFirstLineIndent(0);

        Style style("normal", "正文", StyleType::Paragraph, charFmt, paraFmt);
        style.setCategory(StyleCategory::BuiltIn);
        style.setBuiltIn(true);
        style.setPriority(100);
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // heading1 (标题 1)
    {
        CharacterFormat charFmt;
        charFmt.setFontSize(18);
        charFmt.setBold(true);
        charFmt.setColor(Qt::darkBlue);

        ParagraphFormat paraFmt;
        paraFmt.setSpaceBefore(24);
        paraFmt.setSpaceAfter(12);

        Style style("heading1", "标题 1", StyleType::Paragraph, charFmt, paraFmt, "normal");
        style.setCategory(StyleCategory::Heading);
        style.setBuiltIn(true);
        style.setPriority(90);
        style.setNextStyleId("normal");
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // heading2 (标题 2)
    {
        CharacterFormat charFmt;
        charFmt.setFontSize(16);
        charFmt.setBold(true);
        charFmt.setColor(Qt::darkBlue);

        ParagraphFormat paraFmt;
        paraFmt.setSpaceBefore(18);
        paraFmt.setSpaceAfter(9);

        Style style("heading2", "标题 2", StyleType::Paragraph, charFmt, paraFmt, "normal");
        style.setCategory(StyleCategory::Heading);
        style.setBuiltIn(true);
        style.setPriority(85);
        style.setNextStyleId("normal");
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // heading3 (标题 3)
    {
        CharacterFormat charFmt;
        charFmt.setFontSize(14);
        charFmt.setBold(true);
        charFmt.setColor(Qt::darkBlue);

        ParagraphFormat paraFmt;
        paraFmt.setSpaceBefore(14);
        paraFmt.setSpaceAfter(7);

        Style style("heading3", "标题 3", StyleType::Paragraph, charFmt, paraFmt, "normal");
        style.setCategory(StyleCategory::Heading);
        style.setBuiltIn(true);
        style.setPriority(80);
        style.setNextStyleId("normal");
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // quote (引用)
    {
        CharacterFormat charFmt;
        charFmt.setColor(Qt::darkGray);

        ParagraphFormat paraFmt;
        paraFmt.setFirstLineIndent(28);

        Style style("quote", "引用", StyleType::Paragraph, charFmt, paraFmt, "normal");
        style.setCategory(StyleCategory::Other);
        style.setBuiltIn(true);
        style.setPriority(70);
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // code_block (代码块)
    {
        CharacterFormat charFmt;
        charFmt.setFontFamily("Consolas");
        charFmt.setColor(Qt::darkRed);

        ParagraphFormat paraFmt;
        paraFmt.setFirstLineIndent(28);

        Style style("code_block", "代码块", StyleType::Paragraph, charFmt, paraFmt, "normal");
        style.setCategory(StyleCategory::Other);
        style.setBuiltIn(true);
        style.setPriority(65);
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // ==================== 字符样式 ====================

    // emphasis (强调)
    {
        CharacterFormat charFmt;
        charFmt.setItalic(true);

        Style style("emphasis", "强调", StyleType::Character, charFmt, ParagraphFormat());
        style.setCategory(StyleCategory::Character);
        style.setBuiltIn(true);
        style.setPriority(50);
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // strong (加粗)
    {
        CharacterFormat charFmt;
        charFmt.setBold(true);

        Style style("strong", "加粗", StyleType::Character, charFmt, ParagraphFormat());
        style.setCategory(StyleCategory::Character);
        style.setBuiltIn(true);
        style.setPriority(45);
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    // code (代码)
    {
        CharacterFormat charFmt;
        charFmt.setFontFamily("Consolas");
        charFmt.setColor(Qt::darkRed);

        Style style("code", "代码", StyleType::Character, charFmt, ParagraphFormat());
        style.setCategory(StyleCategory::Character);
        style.setBuiltIn(true);
        style.setPriority(40);
        m_styles.insert(style.id(), style);
        m_defaultStyleIds.append(style.id());
    }

    emit stylesChanged();
}

void StyleManager::resetToDefaults() {
    initializeDefaultStyles();
}

bool StyleManager::isDefaultStyle(const QString &styleId) const {
    return m_defaultStyleIds.contains(styleId);
}

bool StyleManager::addStyle(const Style &style) {
    bool isNew = !m_styles.contains(style.id());
    m_styles.insert(style.id(), style);
    emit stylesChanged();
    emit styleChanged(style.id());
    if (isNew) {
        emit styleAdded(style.id());
    } else {
        emit styleUpdated(style.id());
    }
    return true;
}

bool StyleManager::updateStyle(const Style &style) {
    if (!m_styles.contains(style.id())) {
        return false;
    }
    m_styles.insert(style.id(), style);
    emit stylesChanged();
    emit styleChanged(style.id());
    emit styleUpdated(style.id());
    return true;
}

bool StyleManager::removeStyle(const QString &id) {
    const Style *style = getStyle(id);
    if (!style) return false;
    if (!style->canDelete()) return false;
    
    if (m_styles.remove(id) > 0) {
        emit stylesChanged();
        emit styleChanged(id);
        emit styleRemoved(id);
        return true;
    }
    return false;
}

const Style* StyleManager::getStyle(const QString &id) const {
    auto it = m_styles.find(id);
    if (it != m_styles.end()) {
        return &it.value();
    }
    return nullptr;
}

QList<QString> StyleManager::styleIds() const {
    return m_styles.keys();
}

QList<Style> StyleManager::allStyles() const {
    return m_styles.values();
}

QList<Style> StyleManager::stylesByCategory(StyleCategory category) const {
    QList<Style> result;
    for (const Style &style : m_styles) {
        if (style.isHidden()) continue;
        
        bool match = false;
        switch (category) {
            case StyleCategory::All:
                match = true;
                break;
            case StyleCategory::BuiltIn:
                match = style.isBuiltIn();
                break;
            case StyleCategory::Custom:
                match = !style.isBuiltIn();
                break;
            case StyleCategory::Paragraph:
                match = style.type() == StyleType::Paragraph;
                break;
            case StyleCategory::Character:
                match = style.type() == StyleType::Character;
                break;
            case StyleCategory::Heading:
                match = style.category() == StyleCategory::Heading;
                break;
            case StyleCategory::List:
                match = style.category() == StyleCategory::List;
                break;
            case StyleCategory::Other:
                match = style.category() == StyleCategory::Other;
                break;
        }
        if (match) {
            result.append(style);
        }
    }
    
    std::sort(result.begin(), result.end(), [](const Style &a, const Style &b) {
        return a.priority() > b.priority();
    });
    
    return result;
}

QList<Style> StyleManager::stylesByType(StyleType type) const {
    QList<Style> result;
    for (const Style &style : m_styles) {
        if (style.type() == type && !style.isHidden()) {
            result.append(style);
        }
    }
    return result;
}

QList<Style> StyleManager::searchStyles(const QString &keyword) const {
    QList<Style> result;
    QString lowerKeyword = keyword.toLower();
    for (const Style &style : m_styles) {
        if (style.isHidden()) continue;
        if (style.id().toLower().contains(lowerKeyword) || 
            style.name().toLower().contains(lowerKeyword)) {
            result.append(style);
        }
    }
    return result;
}

QList<Style> StyleManager::availableParentStyles(const QString &currentStyleId) const {
    QList<Style> result;
    for (const Style &style : m_styles) {
        if (style.id() == currentStyleId) continue;
        if (style.isHidden()) continue;
        if (style.type() != m_styles.value(currentStyleId).type()) continue;
        
        if (!hasInheritanceCycle(currentStyleId, style.id())) {
            result.append(style);
        }
    }
    return result;
}

bool StyleManager::isValidStyleId(const QString &styleId) const {
    if (styleId.isEmpty()) return false;
    for (const QChar &c : styleId) {
        if (!c.isLetterOrNumber() && c != '_') {
            return false;
        }
    }
    return true;
}

QString StyleManager::generateUniqueStyleId(const QString &baseName) const {
    QString id = baseName;
    int counter = 1;
    while (m_styles.contains(id)) {
        id = QString("%1_%2").arg(baseName).arg(counter);
        counter++;
    }
    return id;
}

bool StyleManager::hasInheritanceCycle(const QString &styleId) const {
    return hasInheritanceCycle(styleId, QString());
}

bool StyleManager::hasInheritanceCycle(const QString &startId, const QString &currentParentId) const {
    if (currentParentId.isEmpty()) {
        const Style *style = getStyle(startId);
        if (!style || !style->hasParent()) return false;
        return hasInheritanceCycle(startId, style->parentId());
    }
    
    if (currentParentId == startId) return true;
    
    const Style *parentStyle = getStyle(currentParentId);
    if (!parentStyle || !parentStyle->hasParent()) return false;
    
    return hasInheritanceCycle(startId, parentStyle->parentId());
}

CharacterFormat StyleManager::resolveCharacterFormat(const QString &styleId) const {
    const Style* style = getStyle(styleId);
    if (!style) return CharacterFormat();

    CharacterFormat result = style->characterFormat();
    if (!style->parentId().isEmpty()) {
        CharacterFormat parentFormat = resolveCharacterFormat(style->parentId());
        result = CharacterFormat::merge(parentFormat, result);
    }
    return result;
}

ParagraphFormat StyleManager::resolveParagraphFormat(const QString &styleId) const {
    const Style* style = getStyle(styleId);
    if (!style) return ParagraphFormat();

    ParagraphFormat result = style->paragraphFormat();
    if (!style->parentId().isEmpty()) {
        ParagraphFormat parentFormat = resolveParagraphFormat(style->parentId());
        result = ParagraphFormat::merge(parentFormat, result);
    }
    return result;
}

CharacterFormat StyleManager::mergeCharacterFormat(const CharacterFormat &base, const CharacterFormat &direct) {
    return CharacterFormat::merge(base, direct);
}

ParagraphFormat StyleManager::mergeParagraphFormat(const ParagraphFormat &base, const ParagraphFormat &direct) {
    return ParagraphFormat::merge(base, direct);
}

bool StyleManager::loadFromJson(const QJsonObject &json) {
    QJsonArray stylesArray = json["styles"].toArray();
    m_styles.clear();
    
    for (const QJsonValue &value : stylesArray) {
        QJsonObject styleObj = value.toObject();
        
        Style style;
        style.setId(styleObj["id"].toString());
        style.setName(styleObj["name"].toString());
        style.setType(static_cast<StyleType>(styleObj["type"].toInt()));
        style.setCategory(static_cast<StyleCategory>(styleObj["category"].toInt()));
        style.setDescription(styleObj["description"].toString());
        style.setBuiltIn(styleObj["isBuiltIn"].toBool());
        style.setHidden(styleObj["isHidden"].toBool());
        style.setPriority(styleObj["priority"].toInt());
        style.setNextStyleId(styleObj["nextStyleId"].toString());
        style.setParentId(styleObj["parentId"].toString());
        
        m_styles.insert(style.id(), style);
    }
    
    emit stylesChanged();
    return true;
}

QJsonObject StyleManager::saveToJson() const {
    QJsonObject json;
    QJsonArray stylesArray;
    
    for (const Style &style : m_styles) {
        QJsonObject styleObj;
        styleObj["id"] = style.id();
        styleObj["name"] = style.name();
        styleObj["type"] = static_cast<int>(style.type());
        styleObj["category"] = static_cast<int>(style.category());
        styleObj["description"] = style.description();
        styleObj["isBuiltIn"] = style.isBuiltIn();
        styleObj["isHidden"] = style.isHidden();
        styleObj["priority"] = style.priority();
        styleObj["nextStyleId"] = style.nextStyleId();
        styleObj["parentId"] = style.parentId();
        
        stylesArray.append(styleObj);
    }
    
    json["styles"] = stylesArray;
    return json;
}

bool StyleManager::loadFromFile(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        return false;
    }
    
    return loadFromJson(doc.object());
}

bool StyleManager::saveToFile(const QString &filePath) const {
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QJsonObject json = saveToJson();
    QJsonDocument doc(json);
    file.write(doc.toJson());
    file.close();
    
    return true;
}
