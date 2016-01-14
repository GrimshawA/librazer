#include "aeon_expression.h"

float aeon_expression::as_float()
{
	return static_cast<ast_floatexpr*>(this)->value;
}

std::string aeon_expression::as_string()
{
	return static_cast<ast_stringexpr*>(this)->value;
}

int aeon_expression::as_int()
{
	return static_cast<ast_intexpr*>(this)->value;
}

bool aeon_expression::isInt()
{
	return type == IntExpr;
}

bool aeon_expression::isString()
{
	return type == StringExpr;
}

std::string aeon_expression::printtext()
{
	return "Expr";
}

std::string aeon_expression::exprstr()
{
	return "";
}

bool aeon_expression::isFloat()
{
	return type == FloatExpr;
}