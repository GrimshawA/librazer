#include "aeNodeAccessOperator.h"

aeNodeAccessOperator::aeNodeAccessOperator()
: aeNodeExpr()
, m_accessType(Dot)
{
	m_nodeType = (AEN_ACCESSOPERATOR);
}

std::string aeNodeAccessOperator::exprstr()
{
	return "(" + m_a->exprstr() + "." + m_b->exprstr() + ")";
}