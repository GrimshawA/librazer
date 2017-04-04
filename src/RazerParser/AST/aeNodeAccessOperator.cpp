#include "aeNodeAccessOperator.h"

aeNodeAccessOperator::aeNodeAccessOperator()
: aeNodeExpr()
, m_accessType(Dot)
{
	m_nodeType = (AEN_ACCESSOPERATOR);
}

aeQualType aeNodeAccessOperator::getQualifiedType(RzCompiler* c)
{
	// When the left side in a.b is a var, it can only yield another var as result
	aeQualType leftType = m_a->getQualifiedType(c);
	if (leftType.isVariant())
	{
		return leftType;
	}

	return m_b->getQualifiedType(c);
}

std::string aeNodeAccessOperator::str() const
{
	return "(" + m_a->str() + "." + m_b->str() + ")";
}
