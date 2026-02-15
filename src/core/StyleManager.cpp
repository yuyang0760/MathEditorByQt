// ============================================================================
// StyleManager.cpp
// 样式管理器实现文件
// ============================================================================

#include "core/StyleManager.h"
#include <QFont>

StyleManager* StyleManager::m_instance = nullptr;

StyleManager* StyleManager::instance() {
    if (!m_instance) {
        m_instance = new StyleManager();
    }
    return m_instance;
}

StyleManager::StyleManager(QObject *parent) : QObject(parent) {
    // 初始化默认样式：正文样式
    Format defaultFormat(QFont("Microsoft YaHei", 12), Qt::black);
    Style normal("style_normal", "正文", defaultFormat, "");
    m_styles.insert(normal.id(), normal);
}

StyleManager::~StyleManager() {}

bool StyleManager::addStyle(const Style &style) {
    m_styles.insert(style.id(), style);
    emit stylesChanged();
    emit styleChanged(style.id());
    return true;
}

bool StyleManager::removeStyle(const QString &id) {
    if (id == "style_normal") return false; // 禁止删除默认样式
    if (m_styles.remove(id) > 0) {
        emit stylesChanged();
        emit styleChanged(id);
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

Format StyleManager::resolveStyle(const QString &styleId) const {
    const Style* style = getStyle(styleId);
    if (!style) return Format(); // 默认格式

    // 递归解析父样式
    Format result = style->format();
    if (!style->parentId().isEmpty()) {
        Format parentFormat = resolveStyle(style->parentId());
        // 父样式属性作为基础，子样式属性覆盖（若子样式属性有效则保留，否则使用父样式）
        // 这里简单实现：如果子样式的某属性与默认值不同，则认为已覆盖，否则使用父样式
        // 更好的做法是Format中增加"是否已设置"的标志，但为了简化，我们假设所有属性都有默认值。
        // 这里我们使用一种简单的合并：如果子样式的字体与默认字体不同，则使用子字体；否则用父字体。
        // 类似地处理颜色。
        QFont font = result.font();
        if (font.family() == QFont().family() && font.pointSize() == QFont().pointSize() &&
            font.bold() == QFont().bold() && font.italic() == QFont().italic() &&
            font.underline() == QFont().underline()) {
            // 子字体未改变，继承父字体
            font = parentFormat.font();
        }
        QColor color = result.color();
        if (color == QColor()) {
            color = parentFormat.color();
        }
        result = Format(font, color);
    }
    return result;
}

Format StyleManager::mergeFormat(const Format &base, const Format &direct) {
    // 直接格式中的非默认属性覆盖基础格式
    QFont font = base.font();
    if (direct.font().family() != QFont().family())
        font.setFamily(direct.font().family());
    if (direct.font().pointSize() != QFont().pointSize())
        font.setPointSize(direct.font().pointSize());
    font.setBold(direct.bold());
    font.setItalic(direct.italic());
    font.setUnderline(direct.underline());

    QColor color = direct.color().isValid() ? direct.color() : base.color();
    return Format(font, color);
}