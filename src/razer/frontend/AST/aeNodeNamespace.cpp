#include "aeNodeNamespace.h"
#include "AEStructNode.h"
#include <razer/frontend/AST/Nodes.h>

aeNodeNamespace::aeNodeNamespace()
{
	m_nodeType = AEN_NAMESPACE;
}

std::string aeNodeNamespace::str()
{
	return "Namespace";
}