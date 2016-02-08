#include "aeNodeVarDecl.h"
#include "aeNodeExpr.h"

aeNodeVarDecl::aeNodeVarDecl()
{
	m_nodeType = AEN_VARDECL;
}

std::string aeNodeVarDecl::str() const
{
	std::string s = m_type.str();
	s += " " + m_decls[0].m_name;
	/* = "Decl '" + m_type.str() + "' '" + m_name + "'"*/;


	//m_initExpr ? strr += " = " + m_initExpr->exprstr() : strr += "";
	return s;
}
