#include "aeNodeBranch.h"
#include "aeNodeBlock.h"
#include "aeNodeExpr.h"

aeNodeBranch::aeNodeBranch()
{
	m_nodeType = IfBranch;
}

std::string aeNodeBranch::printtext()
{
	return "if " + m_expression->exprstr();
}
