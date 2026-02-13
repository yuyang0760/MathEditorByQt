#include "core/MathObject.h"

MathObject::MathObject(Type type) : m_type(type) {}
MathObject::Type MathObject::type() const { return m_type; }
void MathObject::setLatex(const QString &latex) { m_latex = latex; }
QString MathObject::latex() const { return m_latex; }