// ============================================================================
// MathObject.h
// 数学对象类头文件
// 定义数学公式对象，支持多种数学表达式类型
// ============================================================================

#ifndef MATHOBJECT_H
#define MATHOBJECT_H

#include <QString>

/**
 * @brief 数学对象类
 * 
 * MathObject类表示文档中的数学公式对象，支持多种数学表达式类型。
 * 目前作为占位符实现，未来可扩展为树状结构以支持复杂数学表达式。
 * 
 * 主要功能：
 * - 数学表达式类型管理
 * - LaTeX公式表示
 * - 类型检查和设置
 */
class MathObject {
public:
    /**
     * @brief 数学对象类型枚举
     * 定义支持的数学表达式类型
     */
    enum Type { 
        Placeholder,  ///< 占位符（基本类型）
        Fraction,     ///< 分数
        Radical,      ///< 根式
        Script,       ///< 上下标
        Matrix        ///< 矩阵
    };

    /**
     * @brief 构造函数
     * @param type 数学对象类型（默认为占位符）
     */
    MathObject(Type type = Placeholder);
    
    /**
     * @brief 获取类型
     * @return 当前数学对象类型
     */
    Type type() const;

    /**
     * @brief 设置LaTeX公式
     * @param latex LaTeX格式的公式字符串
     */
    void setLatex(const QString &latex);
    
    /**
     * @brief 获取LaTeX公式
     * @return LaTeX格式的公式字符串
     */
    QString latex() const;

private:
    Type m_type;        ///< 数学对象类型
    QString m_latex;    ///< LaTeX公式表示
};

#endif // MATHOBJECT_H