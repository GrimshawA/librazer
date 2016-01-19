#include "aeNodeVarDecl.h"
#include "aeNodeExpr.h"

aeNodeVarDecl::aeNodeVarDecl()
{
	m_type = AEN_VARDECL;
	init_expr = nullptr;
}

std::string aeNodeVarDecl::printtext()
{
	std::string strr = "Decl '" + type_name + "' '" + name + "'";
	init_expr ? strr += " = " + init_expr->exprstr() : strr += "";
	return strr;
}
