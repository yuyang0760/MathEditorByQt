// ============================================================================
// StyleManager.h
// 样式管理器头文件
// 负责管理所有样式，提供样式查找、解析继承、信号通知等
// ============================================================================

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "Style.h"
#include "CharacterFormat.h"
#include "ParagraphFormat.h"
#include <QObject>
#include <QMap>
#include <QJsonObject>

/**
 * @brief 样式管理器类（单例模式）
 * 
 * 管理文档中所有样式，处理样式的添加、删除、修改、查找，
 * 并提供根据样式ID解析最终格式的功能（考虑继承）。
 */
class StyleManager : public QObject {
    Q_OBJECT
public:
    /**
     * @brief 获取单例实例
     */
    static StyleManager* instance();

    /**
     * @brief 添加或更新样式
     * @param style 样式对象
     * @return 是否成功（若ID重复则覆盖）
     */
    bool addStyle(const Style &style);

    /**
     * @brief 更新样式
     * @param style 样式对象
     * @return 是否成功
     */
    bool updateStyle(const Style &style);

    /**
     * @brief 删除样式
     * @param id 样式ID
     * @return 是否成功
     */
    bool removeStyle(const QString &id);

    /**
     * @brief 获取样式
     * @param id 样式ID
     * @return 样式指针，若不存在返回nullptr
     */
    const Style* getStyle(const QString &id) const;

    /**
     * @brief 获取所有样式ID列表
     */
    QList<QString> styleIds() const;

    /**
     * @brief 获取所有样式（用于界面显示）
     */
    QList<Style> allStyles() const;

    /**
     * @brief 解析样式的最终字符格式（考虑继承）
     * @param styleId 样式ID
     * @return 解析后的字符格式，若样式不存在返回默认格式
     */
    CharacterFormat resolveCharacterFormat(const QString &styleId) const;

    /**
     * @brief 解析样式的最终段落格式（考虑继承）
     * @param styleId 样式ID
     * @return 解析后的段落格式，若样式不存在返回默认格式
     */
    ParagraphFormat resolveParagraphFormat(const QString &styleId) const;

    /**
     * @brief 应用直接字符格式覆盖到基础格式上
     * @param base 基础格式（来自样式）
     * @param direct 直接格式覆盖
     * @return 合并后的格式
     */
    static CharacterFormat mergeCharacterFormat(const CharacterFormat &base, const CharacterFormat &direct);

    /**
     * @brief 应用直接段落格式覆盖到基础格式上
     * @param base 基础格式（来自样式）
     * @param direct 直接格式覆盖
     * @return 合并后的格式
     */
    static ParagraphFormat mergeParagraphFormat(const ParagraphFormat &base, const ParagraphFormat &direct);

    // ==================== 新增功能 ====================

    /**
     * @brief 初始化默认样式
     */
    void initializeDefaultStyles();

    /**
     * @brief 重置为默认样式
     */
    void resetToDefaults();

    /**
     * @brief 检查是否为默认样式
     */
    bool isDefaultStyle(const QString &styleId) const;

    /**
     * @brief 根据分类获取样式
     */
    QList<Style> stylesByCategory(StyleCategory category) const;

    /**
     * @brief 根据类型获取样式
     */
    QList<Style> stylesByType(StyleType type) const;

    /**
     * @brief 搜索样式
     */
    QList<Style> searchStyles(const QString &keyword) const;

    /**
     * @brief 获取可用的父样式
     */
    QList<Style> availableParentStyles(const QString &currentStyleId) const;

    /**
     * @brief 验证样式ID
     */
    bool isValidStyleId(const QString &styleId) const;

    /**
     * @brief 生成唯一的样式ID
     */
    QString generateUniqueStyleId(const QString &baseName = "CustomStyle") const;

    /**
     * @brief 检查继承循环
     */
    bool hasInheritanceCycle(const QString &styleId) const;

    /**
     * @brief 从 JSON 加载
     */
    bool loadFromJson(const QJsonObject &json);

    /**
     * @brief 保存为 JSON
     */
    QJsonObject saveToJson() const;

    /**
     * @brief 从文件加载
     */
    bool loadFromFile(const QString &filePath);

    /**
     * @brief 保存到文件
     */
    bool saveToFile(const QString &filePath) const;

signals:
    /**
     * @brief 样式列表变化信号（添加、删除、修改）
     */
    void stylesChanged();

    /**
     * @brief 特定样式被修改的信号
     * @param styleId 被修改的样式ID
     */
    void styleChanged(const QString &styleId);

    /**
     * @brief 样式添加信号
     */
    void styleAdded(const QString &styleId);

    /**
     * @brief 样式删除信号
     */
    void styleRemoved(const QString &styleId);

    /**
     * @brief 样式更新信号
     */
    void styleUpdated(const QString &styleId);

    /**
     * @brief 样式重命名信号
     */
    void styleRenamed(const QString &oldId, const QString &newId);

private:
    StyleManager(QObject *parent = nullptr);
    ~StyleManager();
    static StyleManager *m_instance;

    /**
     * @brief 检查继承循环（内部方法）
     */
    bool hasInheritanceCycle(const QString &startId, const QString &currentParentId) const;

    QMap<QString, Style> m_styles;  ///< 样式映射
    QList<QString> m_defaultStyleIds; ///< 默认样式ID列表
};

#endif // STYLEMANAGER_H
