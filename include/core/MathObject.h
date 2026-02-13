#ifndef MATHOBJECT_H
#define MATHOBJECT_H

#include <QString>
// 数学公式对象（占位，未来可扩展为树结构）
class MathObject {
public:
    enum Type { Placeholder, Fraction, Radical, Script, Matrix };

    MathObject(Type type = Placeholder);
    Type type() const;

    // 未来可添加子节点、属性等
    void setLatex(const QString &latex);
    QString latex() const;

private:
    Type m_type;
    QString m_latex;
};

#endif // MATHOBJECT_H