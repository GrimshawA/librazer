#include "aeNodeFor.h"
#include "aeNodeExpr.h"
#include "aeNodeBlock.h"

aeNodeFor::aeNodeFor()
{
	m_nodeType = AEN_FOR;
}

std::string aeNodeFor::str() const
{
	std::string s = "for(";
	if (initStatement)
		s += initStatement->str();
	s += ";";
	if (expr)
		s += expr->str();
	s += ";";
	if (incrExpr)
		s += incrExpr->str();
	s += ")";
	return s;
}
