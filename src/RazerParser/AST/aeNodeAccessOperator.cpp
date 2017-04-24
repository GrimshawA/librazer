#include "aeNodeAccessOperator.h"

aeNodeAccessOperator::aeNodeAccessOperator()
: aeNodeExpr()
, m_accessType(Dot)
{
	m_nodeType = (AEN_ACCESSOPERATOR);
}

RzQualType aeNodeAccessOperator::getQualifiedType(RzCompiler* c, RzQualType base)
{
	// When the left side in a.b is a var, it can only yield another var as result
	RzQualType leftType = m_a->getQualifiedType(c);
	if (leftType.isVariant())
	{
		return leftType;
	}

    // Determining the right side is dependent on the left type
    RzQualType lhsType = m_a->getQualifiedType(c);

    return m_b->getQualifiedType(c, lhsType);
}

std::string aeNodeAccessOperator::str() const
{
	return "(" + m_a->str() + "." + m_b->str() + ")";
}
