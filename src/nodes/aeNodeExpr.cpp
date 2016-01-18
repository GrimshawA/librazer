#include "aeNodeExpr.h"
#include "../aeon_expression.h"

aeNodeExpr::aeNodeExpr()
{
	m_parentExpr = nullptr;
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
	return m_type == IntExpr;
}

bool aeNodeExpr::isString()
{
	return m_type == StringExpr;
}

std::string aeNodeExpr::printtext()
{
	return "Expr";
}

std::string aeNodeExpr::exprstr()
{
	return "";
}

bool aeNodeExpr::isFloat()
{
	return m_type == FloatExpr;
}