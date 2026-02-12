// ============================================================================
// Format.h
// 格式类的头文件
// 表示文本的格式，包含字体、颜色等属性
// ============================================================================

#ifndef FORMAT_H
#define FORMAT_H

#include <QFont>
#include <QColor>

/**
 * @class Format
 * @brief 格式类
 * 
 * 表示文本的格式，包含字体、颜色等属性。
 * 用于设置和获取文本的样式信息。
 */
class Format
{
public:
    /**
     * @brief 构造函数
     * 创建一个默认格式
     */
    Format();
    
    /**
     * @brief 构造函数
     * @param font 字体
     * @param color 颜色，默认为黑色
     */
    Format(const QFont &font, const QColor &color = Qt::black);
    
    /**
     * @brief 获取字体
     * @return 字体
     */
    QFont font() const;
    
    /**
     * @brief 设置字体
     * @param font 字体
     */
    void setFont(const QFont &font);
    
    /**
     * @brief 获取颜色
     * @return 颜色
     */
    QColor color() const;
    
    /**
     * @brief 设置颜色
     * @param color 颜色
     */
    void setColor(const QColor &color);
    
    /**
     * @brief 检查是否为粗体
     * @return 是否为粗体
     */
    bool bold() const;
    
    /**
     * @brief 设置粗体
     * @param bold 是否为粗体
     */
    void setBold(bool bold);
    
    /**
     * @brief 检查是否为斜体
     * @return 是否为斜体
     */
    bool italic() const;
    
    /**
     * @brief 设置斜体
     * @param italic 是否为斜体
     */
    void setItalic(bool italic);
    
    /**
     * @brief 检查是否有下划线
     * @return 是否有下划线
     */
    bool underline() const;
    
    /**
     * @brief 设置下划线
     * @param underline 是否有下划线
     */
    void setUnderline(bool underline);
    
    /**
     * @brief 比较两个格式是否相等
     * @param other 另一个格式
     * @return 是否相等
     */
    bool operator==(const Format &other) const;
    
    /**
     * @brief 比较两个格式是否不相等
     * @param other 另一个格式
     * @return 是否不相等
     */
    bool operator!=(const Format &other) const;
    
private:
    /**
     * @brief 字体
     */
    QFont m_font;
    
    /**
     * @brief 颜色
     */
    QColor m_color;
};

#endif // FORMAT_H
