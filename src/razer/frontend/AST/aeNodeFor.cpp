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
	s += ")\n";
	s += block->str();
	return s;
}


RzRangeForNode::RzRangeForNode()
{
    m_nodeType = AEN_RANGEFOR;
}

RzRangeForNode::RzRangeForNode(aeNodeExpr* identity, aeNodeExpr* range)
{
    this->iterator.reset(identity);
    this->range.reset(range);
}

std::string RzRangeForNode::str() const
{
    return {};
}
