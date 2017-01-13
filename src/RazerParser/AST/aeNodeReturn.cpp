#include "aeNodeReturn.h"
#include "AEStructNode.h"
#include <RazerParser/AST/Nodes.h>


aeNodeReturn::aeNodeReturn()
{
	m_nodeType = AEN_RETURN;
}

std::string aeNodeReturn::str()
{
	return std::string("Return");
}