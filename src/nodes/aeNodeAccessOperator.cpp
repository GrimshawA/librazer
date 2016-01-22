#include "aeNodeAccessOperator.h"

aeNodeAccessOperator::aeNodeAccessOperator()
: aeNodeExpr()
, m_accessType(Dot)
{
	m_nodeType = (AEN_ACCESSOPERATOR);
}

aeQualType aeNodeAccessOperator::getQualifiedType(aeon_compiler* c)
{
	return m_b->getQualifiedType(c);
}

std::string aeNodeAccessOperator::exprstr()
{
	return "(" + m_a->exprstr() + "." + m_b->exprstr() + ")";
}
