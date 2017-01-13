#include "RzSourceUnit.h"
#include "AEStructNode.h"
#include <RazerParser/AST/Nodes.h>

void AEBaseNode::add(AEBaseNode* n)
{
	m_items.push_back(n);
}

std::string AEBaseNode::str() const
{
	return ".";
}

void AEBaseNode::printSelf(int tabs)
{
	/*printf("%s%s\n", makeTabbing(tabs).c_str(), str().c_str());
	for (auto item : m_items)
	{
		item->printSelf(tabs + 1);
	}*/
}

////////////////////////////////////////////////////////////////////////// 

bool AEStmtNode::isNullStatement()
{
	return true;
}