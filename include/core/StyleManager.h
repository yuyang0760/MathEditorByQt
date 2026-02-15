// ============================================================================
// StyleManager.h
// 样式管理器头文件
// 负责管理所有样式，提供样式查找、解析继承、信号通知等
// ============================================================================

#ifndef STYLEMANAGER_H
#define STYLEMANAGER_H

#include "Style.h"
#include <QObject>
#include <QMap>

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
     * @brief 解析样式的最终格式（考虑继承）
     * @param styleId 样式ID
     * @return 解析后的格式，若样式不存在返回默认格式
     */
    Format resolveStyle(const QString &styleId) const;

    /**
     * @brief 应用直接格式覆盖到基础格式上
     * @param base 基础格式（来自样式）
     * @param direct 直接格式覆盖（若某属性无效，则使用base的对应属性）
     * @return 合并后的格式
     */
    static Format mergeFormat(const Format &base, const Format &direct);

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

private:
    StyleManager(QObject *parent = nullptr);
    ~StyleManager();
    static StyleManager *m_instance;

    QMap<QString, Style> m_styles;  ///< 样式映射
};

#endif // STYLEMANAGER_H