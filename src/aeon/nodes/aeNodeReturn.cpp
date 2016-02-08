#include "aeNodeReturn.h"
#include "aeNodeClass.h"
#include "../aeon_tree.h"


aeNodeReturn::aeNodeReturn()
{
	m_nodeType = AEN_RETURN;
}

std::string aeNodeReturn::str()
{
	return std::string("Return");
}