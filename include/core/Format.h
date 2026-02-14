/*! \file Format.h
    \brief 格式类头文件
    \author 项目开发者
    \date 2026-02-14
    
    \details 定义文本格式属性，包括字体、颜色、样式等
*/

#ifndef FORMAT_H
#define FORMAT_H

#include <QFont>
#include <QColor>

/*! \class Format
    \brief 格式类
    
    Format类封装了文本的显示属性，包括字体、颜色、粗体、斜体、下划线等。
    它提供了丰富的格式设置接口，支持文本样式的灵活控制。
    
    \section format_features 主要功能
    - 字体和颜色管理
    - 文字样式控制（粗体、斜体、下划线）
    - 格式比较和相等性判断
    
    \section format_usage 使用示例
    \code
    Format fmt(QFont("Arial", 12), Qt::blue);
    fmt.setBold(true);
    fmt.setUnderline(true);
    \endcode
*/
class Format {
public:
    /*! \brief 默认构造函数
        
        创建默认格式（12号微软雅黑字体，黑色）
    */
    Format();
    
    /*! \brief 带参数构造函数
        
        \param font 字体对象
        \param color 颜色对象（默认为黑色）
    */
    Format(const QFont &font, const QColor &color = Qt::black);

    /*! \brief 获取字体
        
        \return 当前字体对象
    */
    QFont font() const;
    
    /*! \brief 设置字体
        
        \param font 新的字体对象
    */
    void setFont(const QFont &font);
    
    /*! \brief 获取颜色
        
        \return 当前颜色对象
    */
    QColor color() const;
    
    /*! \brief 设置颜色
        
        \param color 新的颜色对象
    */
    void setColor(const QColor &color);

    /*! \brief 检查是否为粗体
        
        \return 如果是粗体返回true，否则返回false
    */
    bool bold() const;
    
    /*! \brief 设置粗体样式
        
        \param bold 是否设置为粗体
    */
    void setBold(bool bold);
    
    /*! \brief 检查是否为斜体
        
        \return 如果是斜体返回true，否则返回false
    */
    bool italic() const;
    
    /*! \brief 设置斜体样式
        
        \param italic 是否设置为斜体
    */
    void setItalic(bool italic);
    
    /*! \brief 检查是否有下划线
        
        \return 如果有下划线返回true，否则返回false
    */
    bool underline() const;
    
    /*! \brief 设置下划线样式
        
        \param underline 是否设置下划线
    */
    void setUnderline(bool underline);

    /*! \brief 等于操作符重载
        
        \param other 要比较的另一个格式对象
        \return 如果两个格式相同返回true，否则返回false
    */
    bool operator==(const Format &other) const;
    
    /*! \brief 不等于操作符重载
        
        \param other 要比较的另一个格式对象
        \return 如果两个格式不同返回true，否则返回false
    */
    bool operator!=(const Format &other) const;

private:
    QFont m_font;    ///< 字体属性
    QColor m_color;  ///< 颜色属性
};

#endif // FORMAT_H