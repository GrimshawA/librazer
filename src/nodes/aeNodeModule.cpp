#include "aeNodeModule.h"
#include "aeNodeClass.h"
#include "aeNodeNamespace.h"
#include "../aeon_tree.h"

aeNodeModule::aeNodeModule()
{
	m_type = AEN_MODULE;
}

void aeNodeModule::printSelf(int tabs)
{
	printf("%sModule '%s'\n", makeTabbing(tabs).c_str(), m_name.c_str());
	for (auto item : m_items)
	{
		item->printSelf(tabs + 1);
	}
}