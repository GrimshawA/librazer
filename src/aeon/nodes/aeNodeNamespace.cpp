#include "aeNodeNamespace.h"
#include "aeNodeClass.h"
#include "../aeon_tree.h"

aeNodeNamespace::aeNodeNamespace()
{
	m_nodeType = AEN_NAMESPACE;
}

std::string aeNodeNamespace::str()
{
	return "Namespace";
}