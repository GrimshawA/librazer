#include "RzSourceUnit.h"
#include "AEStructNode.h"
#include "aeNodeNamespace.h"
#include <RazerParser/AST/Nodes.h>

RzSourceUnit::RzSourceUnit()
{
	m_nodeType = AEN_MODULE;
}

std::string RzSourceUnit::str() const {
    std::string s = "ParseRoot [\n";
    for (auto& t : m_types) {
        s += t->str();
    }
    s += "] AST End;";
    return s;
}

void RzSourceUnit::printSelf(int tabs)
{
	
}
