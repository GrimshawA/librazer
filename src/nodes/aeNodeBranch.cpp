#include "aeNodeBranch.h"
#include "aeNodeBlock.h"
#include "aeNodeExpr.h"

aeNodeBranch::aeNodeBranch()
{
	m_type = IfBranch;
}

std::string aeNodeBranch::printtext()
{
	return "if " + m_expression->exprstr();
}
