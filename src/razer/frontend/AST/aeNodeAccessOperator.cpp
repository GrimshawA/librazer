#include "aeNodeAccessOperator.h"

aeNodeAccessOperator::aeNodeAccessOperator()
: aeNodeExpr()
, m_accessType(Dot)
{
	m_nodeType = (AEN_ACCESSOPERATOR);
}

std::string aeNodeAccessOperator::str() const
{
	return "(" + m_a->str() + "." + m_b->str() + ")";
}
