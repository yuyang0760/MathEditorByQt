// ============================================================================
// TextRun.cpp
// 文本片段类实现文件
// 
// TextRun 类表示文档中的一个文本片段，包含文本内容和格式信息。
// 支持样式继承和直接格式设置两种格式应用方式。
// ============================================================================

#include "core/TextRun.h"
#include "core/StyleManager.h"

// ============================================================================
// 构造函数实现
// ============================================================================

/**
 * @brief 默认构造函数
 * 创建一个空的文本片段，不包含直接格式
 */
TextRun::TextRun() : m_hasDirectFormat(false) {}

/**
 * @brief 带文本和直接格式的构造函数
 * @param text 文本内容
 * @param format 直接格式设置
 * 
 * 创建包含指定文本和直接格式的文本片段
 */
TextRun::TextRun(const QString &text, const Format &format)
    : m_text(text), m_directFormat(format), m_hasDirectFormat(true) {}

/**
 * @brief 完整构造函数
 * @param text 文本内容
 * @param styleId 样式标识符
 * @param directFormat 直接格式设置
 * 
 * 创建包含文本、样式引用和直接格式的文本片段
 * 直接格式会覆盖样式中的相应属性
 */
TextRun::TextRun(const QString &text, const QString &styleId, const Format &directFormat)
    : m_text(text), m_styleId(styleId), m_directFormat(directFormat), m_hasDirectFormat(true) {}

// ============================================================================
// 文本内容访问器
// ============================================================================

/**
 * @brief 获取文本内容
 * @return 当前文本片段的内容
 */
QString TextRun::text() const { return m_text; }

/**
 * @brief 设置文本内容
 * @param text 新的文本内容
 */
void TextRun::setText(const QString &text) { m_text = text; }

// ============================================================================
// 格式管理方法
// ============================================================================

/**
 * @brief 获取格式（兼容性接口）
 * @return 直接格式，如果没有则返回默认格式
 * 
 * 保持向后兼容性的接口，优先返回直接格式
 */
Format TextRun::format() const {
    return m_hasDirectFormat ? m_directFormat : Format();
}

/**
 * @brief 设置直接格式
 * @param format 要设置的格式
 * 
 * 设置直接格式并标记该文本片段具有直接格式
 */
void TextRun::setFormat(const Format &format) {
    m_directFormat = format;
    m_hasDirectFormat = true;
}

// ============================================================================
// 文本编辑方法
// ============================================================================

/**
 * @brief 在指定位置插入文本
 * @param position 插入位置
 * @param text 要插入的文本
 * 
 * 在文本片段的指定位置插入新的文本内容
 */
void TextRun::insert(int position, const QString &text) {
    m_text.insert(position, text);
}

/**
 * @brief 删除指定范围的文本
 * @param position 删除起始位置
 * @param length 要删除的字符数
 * 
 * 从指定位置开始删除指定长度的文本
 */
void TextRun::remove(int position, int length) {
    m_text.remove(position, length);
}

/**
 * @brief 获取文本长度
 * @return 文本片段中字符的数量
 */
int TextRun::length() const { return m_text.length(); }

// ============================================================================
// 样式管理方法
// ============================================================================

/**
 * @brief 获取样式标识符
 * @return 当前应用的样式ID，如果未设置则返回空字符串
 */
QString TextRun::styleId() const { return m_styleId; }

/**
 * @brief 设置样式标识符
 * @param styleId 要应用的样式ID
 * 
 * 设置文本片段要继承的样式，不会影响已有的直接格式
 */
void TextRun::setStyleId(const QString &styleId) { m_styleId = styleId; }

// ============================================================================
// 直接格式管理方法
// ============================================================================

/**
 * @brief 检查是否具有直接格式
 * @return true 如果设置了直接格式，false 表示仅使用样式继承
 */
bool TextRun::hasDirectFormat() const { return m_hasDirectFormat; }

/**
 * @brief 获取直接格式
 * @return 当前的直接格式设置
 * 
 * 注意：调用前应先检查 hasDirectFormat()
 */
Format TextRun::directFormat() const { return m_directFormat; }

/**
 * @brief 设置直接格式
 * @param format 要设置的直接格式
 * 
 * 设置直接格式并标记该文本片段具有直接格式
 * 直接格式将覆盖样式中的相应属性
 */
void TextRun::setDirectFormat(const Format &format) {
    m_directFormat = format;
    m_hasDirectFormat = true;
}

/**
 * @brief 清除直接格式
 * 
 * 移除直接格式设置，使文本片段仅使用样式继承
 * 不会影响样式ID的设置
 */
void TextRun::clearDirectFormat() { m_hasDirectFormat = false; }

// ============================================================================
// 有效格式计算
// ============================================================================

/**
 * @brief 计算有效的格式设置
 * @param styleMgr 样式管理器指针
 * @return 最终生效的格式
 * 
 * 根据样式继承和直接格式计算最终的格式效果：
 * 1. 首先获取基础样式格式（如果有样式ID且样式管理器存在）
 * 2. 如果有直接格式，则将其与基础样式合并（直接格式优先）
 * 3. 返回最终的有效格式
 */
Format TextRun::effectiveFormat(StyleManager *styleMgr) const {
    Format base;
    // 解析基础样式
    if (!m_styleId.isEmpty() && styleMgr) {
        base = styleMgr->resolveStyle(m_styleId);
    }
    // 合并直接格式（如果存在）
    if (m_hasDirectFormat) {
        return StyleManager::mergeFormat(base, m_directFormat);
    }
    return base;
}