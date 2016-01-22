#include "aeNodeSubscript.h"

aeNodeSubscript::aeNodeSubscript()
{
	m_nodeType = AEN_SUBSCRIPT;
}

std::string aeNodeSubscript::exprstr()
{
	return "(" + subject->exprstr() + ")[" + argument->exprstr() + "]";
}