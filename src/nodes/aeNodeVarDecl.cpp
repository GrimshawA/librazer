#include "aeNodeVarDecl.h"
#include "aeNodeExpr.h"

aeNodeVarDecl::aeNodeVarDecl()
{
	m_nodeType = AEN_VARDECL;
	m_initExpr = nullptr;
}

std::string aeNodeVarDecl::printtext() const
{
	std::string strr = "Decl '" + m_type.str() + "' '" + m_name + "'";
	m_initExpr ? strr += " = " + m_initExpr->exprstr() : strr += "";
	return strr;
}
