#include "aeNodeBlock.h"

aeNodeBlock::aeNodeBlock()
{
	m_type = CodeBlock;
}

void aeNodeBlock::printSelf(int tabs)
{
	/*printf("%sBlock '%s'\n", makeTabbing(tabs).c_str(), "{}");
	for (auto item : m_items)
	{
		item->printSelf(tabs + 1);
	}*/
}