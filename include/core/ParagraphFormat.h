// ============================================================================
// ParagraphFormat.h
// 段落格式类头文件
// 定义文本的段落格式属性，支持未设置状态
// ============================================================================

#ifndef PARAGRAPHFORMAT_H
#define PARAGRAPHFORMAT_H

#include <Qt>
#include "PropertyState.h"

/**
 * @brief 段落对齐方式枚举
 */
enum class Alignment {
    Left,
    Center,
    Right,
    Justify
};

/**
 * @brief 行间距类型枚举
 */
enum class LineSpacingType {
    Single,        ///< 单倍行距
    OneAndHalf,    ///< 1.5倍行距
    Double,        ///< 双倍行距
    Multiple,      ///< 多倍行距（可自定义倍数）
    Minimum,       ///< 最小值
    Exactly        ///< 固定值
};

/**
 * @brief 段落格式类
 * 
 * ParagraphFormat类封装了文本的段落格式属性，包括对齐方式、行间距、段前段后间距等。
 * 每个属性都支持三种状态：未设置、设置为特定值。
 * 未设置的属性将继承自父格式。
 */
class ParagraphFormat {
public:
    /**
     * @brief 默认构造函数
     * 
     * 创建未设置任何属性的段落格式
     */
    ParagraphFormat();

    /**
     * @brief 获取对齐方式
     * 
     * @return 对齐方式，Left表示未设置
     */
    Alignment alignment() const;
    
    /**
     * @brief 设置对齐方式
     * 
     * @param align 对齐方式
     */
    void setAlignment(Alignment align);
    
    /**
     * @brief 检查是否设置了对齐方式
     * 
     * @return 如果设置了对齐方式返回true
     */
    bool hasAlignment() const;
    
    /**
     * @brief 清除对齐方式设置
     */
    void clearAlignment();

    /**
     * @brief 获取行间距
     * 
     * @return 行间距倍数，0表示未设置
     */
    qreal lineSpacing() const;
    
    /**
     * @brief 设置行间距
     * 
     * @param spacing 行间距倍数
     */
    void setLineSpacing(qreal spacing);
    
    /**
     * @brief 检查是否设置了行间距
     * 
     * @return 如果设置了行间距返回true
     */
    bool hasLineSpacing() const;
    
    /**
     * @brief 清除行间距设置
     */
    void clearLineSpacing();

    /**
     * @brief 获取行间距类型
     * 
     * @return 行间距类型，Single表示未设置
     */
    LineSpacingType lineSpacingType() const;
    
    /**
     * @brief 设置行间距类型
     * 
     * @param type 行间距类型
     */
    void setLineSpacingType(LineSpacingType type);
    
    /**
     * @brief 检查是否设置了行间距类型
     * 
     * @return 如果设置了行间距类型返回true
     */
    bool hasLineSpacingType() const;
    
    /**
     * @brief 清除行间距类型设置
     */
    void clearLineSpacingType();

    /**
     * @brief 获取行高（像素）
     * 
     * @return 行高（像素），0表示未设置
     */
    qreal lineHeight() const;
    
    /**
     * @brief 设置行高（像素）
     * 
     * @param height 行高（像素）
     */
    void setLineHeight(qreal height);
    
    /**
     * @brief 检查是否设置了行高
     * 
     * @return 如果设置了行高返回true
     */
    bool hasLineHeight() const;
    
    /**
     * @brief 清除行高设置
     */
    void clearLineHeight();

    /**
     * @brief 获取段前间距
     * 
     * @return 段前间距（像素），0表示未设置
     */
    int spaceBefore() const;
    
    /**
     * @brief 设置段前间距
     * 
     * @param space 段前间距（像素）
     */
    void setSpaceBefore(int space);
    
    /**
     * @brief 检查是否设置了段前间距
     * 
     * @return 如果设置了段前间距返回true
     */
    bool hasSpaceBefore() const;
    
    /**
     * @brief 清除段前间距设置
     */
    void clearSpaceBefore();

    /**
     * @brief 获取段后间距
     * 
     * @return 段后间距（像素），0表示未设置
     */
    int spaceAfter() const;
    
    /**
     * @brief 设置段后间距
     * 
     * @param space 段后间距（像素）
     */
    void setSpaceAfter(int space);
    
    /**
     * @brief 检查是否设置了段后间距
     * 
     * @return 如果设置了段后间距返回true
     */
    bool hasSpaceAfter() const;
    
    /**
     * @brief 清除段后间距设置
     */
    void clearSpaceAfter();

    /**
     * @brief 获取首行缩进
     * 
     * @return 首行缩进（像素），0表示未设置
     */
    int firstLineIndent() const;
    
    /**
     * @brief 设置首行缩进
     * 
     * @param indent 首行缩进（像素）
     */
    void setFirstLineIndent(int indent);
    
    /**
     * @brief 检查是否设置了首行缩进
     * 
     * @return 如果设置了首行缩进返回true
     */
    bool hasFirstLineIndent() const;
    
    /**
     * @brief 清除首行缩进设置
     */
    void clearFirstLineIndent();

    /**
     * @brief 合并两个段落格式
     * 
     * 将override格式的已设置属性合并到base格式中
     * 
     * @param base 基础格式
     * @param override 覆盖格式
     * @return 合并后的格式
     */
    static ParagraphFormat merge(const ParagraphFormat &base, const ParagraphFormat &override);

    /**
     * @brief 等于操作符重载
     * 
     * @param other 要比较的另一个格式对象
     * @return 如果两个格式相同返回true
     */
    bool operator==(const ParagraphFormat &other) const;
    
    /**
     * @brief 不等于操作符重载
     * 
     * @param other 要比较的另一个格式对象
     * @return 如果两个格式不同返回true
     */
    bool operator!=(const ParagraphFormat &other) const;

private:
    Alignment m_alignment;
    bool m_hasAlignment;
    
    LineSpacingType m_lineSpacingType;
    bool m_hasLineSpacingType;
    
    qreal m_lineSpacing;
    bool m_hasLineSpacing;
    
    qreal m_lineHeight;
    bool m_hasLineHeight;
    
    int m_spaceBefore;
    bool m_hasSpaceBefore;
    
    int m_spaceAfter;
    bool m_hasSpaceAfter;
    
    int m_firstLineIndent;
    bool m_hasFirstLineIndent;
};

#endif // PARAGRAPHFORMAT_H
