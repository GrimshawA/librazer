#include "aeNodeExpr.h"
#include "aeNodeLiterals.h"
#include <RazerCompiler/RzCompiler.h>

aeNodeExpr::aeNodeExpr()
{
	m_parentExpr = nullptr;
}

aeNodeExpr* aeNodeExpr::getParentExpression()
{
	return m_parentExpr;
}

float aeNodeExpr::as_float()
{
	return static_cast<aeNodeFloat*>(this)->value;
}

std::string aeNodeExpr::as_string()
{
	return static_cast<aeNodeString*>(this)->value;
}

int aeNodeExpr::as_int()
{
	return static_cast<aeNodeInteger*>(this)->value;
}

bool aeNodeExpr::isInt()
{
	return m_nodeType == AEN_INTEGER;
}

bool aeNodeExpr::isString()
{
	return m_nodeType == AEN_STRING;
}

std::string aeNodeExpr::str() const
{
	return "nullexpr";
}

bool aeNodeExpr::isFloat()
{
	return m_nodeType == AEN_FLOAT;
}
