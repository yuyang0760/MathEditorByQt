// ============================================================================
// CharacterFormat.h
// 字符格式类头文件
// 定义文本的字符格式属性，支持未设置状态
// ============================================================================

#ifndef CHARACTERFORMAT_H
#define CHARACTERFORMAT_H

#include <QFont>
#include <QColor>
#include "PropertyState.h"

/**
 * @brief 字符格式类
 * 
 * CharacterFormat类封装了文本的字符显示属性，包括字体、颜色、粗体、斜体、下划线等。
 * 每个属性都支持三种状态：未设置、设置为true、设置为false。
 * 未设置的属性将继承自父格式。
 */
class CharacterFormat {
public:
    /**
     * @brief 默认构造函数
     * 
     * 创建未设置任何属性的字符格式
     */
    CharacterFormat();
    
    /**
     * @brief 带参数构造函数
     * 
     * @param font 字体对象
     * @param color 颜色对象（默认为黑色）
     */
    CharacterFormat(const QFont &font, const QColor &color = Qt::black);

    /**
     * @brief 获取字体族
     * 
     * @return 字体族名称，空字符串表示未设置
     */
    QString fontFamily() const;
    
    /**
     * @brief 设置字体族
     * 
     * @param family 字体族名称
     */
    void setFontFamily(const QString &family);
    
    /**
     * @brief 检查是否设置了字体族
     * 
     * @return 如果设置了字体族返回true
     */
    bool hasFontFamily() const;
    
    /**
     * @brief 清除字体族设置
     */
    void clearFontFamily();

    /**
     * @brief 获取字号
     * 
     * @return 字号，0表示未设置
     */
    int fontSize() const;
    
    /**
     * @brief 设置字号
     * 
     * @param size 字号
     */
    void setFontSize(int size);
    
    /**
     * @brief 检查是否设置了字号
     * 
     * @return 如果设置了字号返回true
     */
    bool hasFontSize() const;
    
    /**
     * @brief 清除字号设置
     */
    void clearFontSize();

    /**
     * @brief 获取颜色
     * 
     * @return 颜色对象，无效颜色表示未设置
     */
    QColor color() const;
    
    /**
     * @brief 设置颜色
     * 
     * @param color 颜色对象
     */
    void setColor(const QColor &color);
    
    /**
     * @brief 检查是否设置了颜色
     * 
     * @return 如果设置了颜色返回true
     */
    bool hasColor() const;
    
    /**
     * @brief 清除颜色设置
     */
    void clearColor();

    /**
     * @brief 获取粗体状态
     * 
     * @return 粗体属性状态
     */
    PropertyState boldState() const;
    
    /**
     * @brief 设置粗体
     * 
     * @param bold 是否设置为粗体
     */
    void setBold(bool bold);
    
    /**
     * @brief 检查是否设置了粗体
     * 
     * @return 如果设置了粗体返回true
     */
    bool hasBold() const;
    
    /**
     * @brief 清除粗体设置
     */
    void clearBold();

    /**
     * @brief 获取斜体状态
     * 
     * @return 斜体属性状态
     */
    PropertyState italicState() const;
    
    /**
     * @brief 设置斜体
     * 
     * @param italic 是否设置为斜体
     */
    void setItalic(bool italic);
    
    /**
     * @brief 检查是否设置了斜体
     * 
     * @return 如果设置了斜体返回true
     */
    bool hasItalic() const;
    
    /**
     * @brief 清除斜体设置
     */
    void clearItalic();

    /**
     * @brief 获取下划线状态
     * 
     * @return 下划线属性状态
     */
    PropertyState underlineState() const;
    
    /**
     * @brief 设置下划线
     * 
     * @param underline 是否设置下划线
     */
    void setUnderline(bool underline);
    
    /**
     * @brief 检查是否设置了下划线
     * 
     * @return 如果设置了下划线返回true
     */
    bool hasUnderline() const;
    
    /**
     * @brief 清除下划线设置
     */
    void clearUnderline();

    /**
     * @brief 合并两个字符格式
     * 
     * 将override格式的已设置属性合并到base格式中
     * 
     * @param base 基础格式
     * @param override 覆盖格式
     * @return 合并后的格式
     */
    static CharacterFormat merge(const CharacterFormat &base, const CharacterFormat &override);

    /**
     * @brief 转换为QFont对象
     * 
     * @param defaultFont 当属性未设置时使用的默认字体
     * @return 转换后的字体对象
     */
    QFont toFont(const QFont &defaultFont = QFont()) const;

    /**
     * @brief 等于操作符重载
     * 
     * @param other 要比较的另一个格式对象
     * @return 如果两个格式相同返回true
     */
    bool operator==(const CharacterFormat &other) const;
    
    /**
     * @brief 不等于操作符重载
     * 
     * @param other 要比较的另一个格式对象
     * @return 如果两个格式不同返回true
     */
    bool operator!=(const CharacterFormat &other) const;

private:
    QString m_fontFamily;
    bool m_hasFontFamily;
    
    int m_fontSize;
    bool m_hasFontSize;
    
    QColor m_color;
    bool m_hasColor;
    
    PropertyState m_boldState;
    PropertyState m_italicState;
    PropertyState m_underlineState;
};

#endif // CHARACTERFORMAT_H
