#include "aeNodeModule.h"
#include "aeNodeClass.h"
#include "../aeon_tree.h"

void aeNodeBase::add(aeNodeBase* n)
{
	m_items.push_back(n);
}

std::string aeNodeBase::printtext()
{
	return ".";
}

void aeNodeBase::printSelf(int tabs)
{
	printf("%s%s\n", makeTabbing(tabs).c_str(), printtext().c_str());
	for (auto item : m_items)
	{
		item->printSelf(tabs + 1);
	}
}