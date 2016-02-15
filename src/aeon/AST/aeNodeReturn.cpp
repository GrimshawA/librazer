#include "aeNodeReturn.h"
#include "aeNodeClass.h"
#include <AEON/AST/Nodes.h>


aeNodeReturn::aeNodeReturn()
{
	m_nodeType = AEN_RETURN;
}

std::string aeNodeReturn::str()
{
	return std::string("Return");
}