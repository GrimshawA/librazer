#include "aeNodeBranch.h"
#include "aeNodeBlock.h"
#include "aeNodeExpr.h"

aeNodeBranch::aeNodeBranch()
{
	m_nodeType = AEN_BRANCH;
}

std::string aeNodeBranch::str() const
{
    std::string s = "if " + m_expression->str() + "\n";
    s += m_block->str() + "\n";
    return s;
}
