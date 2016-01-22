#include "aeNodeBranch.h"
#include "aeNodeBlock.h"
#include "aeNodeExpr.h"

aeNodeBranch::aeNodeBranch()
{
	m_nodeType = AEN_BRANCH;
}

std::string aeNodeBranch::printtext()
{
	return "if " + m_expression->exprstr();
}
