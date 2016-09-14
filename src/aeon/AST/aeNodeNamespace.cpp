#include "aeNodeNamespace.h"
#include "AEStructNode.h"
#include <AEON/AST/Nodes.h>

aeNodeNamespace::aeNodeNamespace()
{
	m_nodeType = AEN_NAMESPACE;
}

std::string aeNodeNamespace::str()
{
	return "Namespace";
}