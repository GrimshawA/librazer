#include "aeNodeNamespace.h"
#include "aeNodeClass.h"
#include <AEON/AST/Nodes.h>

aeNodeNamespace::aeNodeNamespace()
{
	m_nodeType = AEN_NAMESPACE;
}

std::string aeNodeNamespace::str()
{
	return "Namespace";
}