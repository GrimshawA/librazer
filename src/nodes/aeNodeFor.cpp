#include "aeNodeFor.h"
#include "aeNodeExpr.h"
#include "aeNodeBlock.h"

aeNodeFor::aeNodeFor()
{
	m_nodeType = ForLoop;

	//block = new aeNodeBlock();
	//add(block);
}

std::string aeNodeFor::printtext()
{
	return std::string("For ") + expr->printtext() + ";" + incrExpr->printtext();
}
