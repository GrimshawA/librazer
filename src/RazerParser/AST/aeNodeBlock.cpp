#include "aeNodeBlock.h"
#include <razer/ir/ir.hpp>

aeNodeBlock::aeNodeBlock()
{
	m_nodeType = AEN_BLOCK;
}

void aeNodeBlock::emitIR(IRBuilder& builder)
{
    builder.beginBlock();

    for (auto& item: m_items) {

        switch(item->m_nodeType) {
            case AEN_FOR:

                break;

            case AEN_ACCESSOPERATOR:

                break;
        }

        if (item->m_nodeType == AEN_BINARYOP)
        {
            builder.Assign();
        }
        else
            builder.Dummy();
    }

    builder.endBlock();
}

std::string aeNodeBlock::str() const {
    std::string s;
    for (auto item : m_items)
    {
        s += item->str() + "\n";
    }
    return s;
}

void aeNodeBlock::printSelf(int tabs)
{
	/*printf("%sBlock '%s'\n", makeTabbing(tabs).c_str(), "{}");
	for (auto item : m_items)
	{
		item->printSelf(tabs + 1);
	}*/
}
