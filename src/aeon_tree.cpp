#include "aeon_tree.h"
#include "aeon_expression.h"

aeNodeBranch::aeNodeBranch()
{
	m_type = IfBranch;

	// we have to have an expression and block so..
	block = new aeNodeBlock();
	add(block);
}

std::string aeNodeBranch::printtext()
{
	return "if " + expr->exprstr();
}

//////////////////////////////////////////////////////////////////////////

aeNodeFunction::aeNodeFunction()
{
	m_type = AEN_FUNCTION;

	m_block = new aeNodeBlock();
	add(m_block);

	is_global = false;
	is_method = false;
	is_static = false;
	is_destructor = false;
	is_constructor = false;
	is_anon = false;
}

bool aeNodeFunction::isNonStaticMethod()
{
	return is_method && !is_static;
}

bool aeNodeFunction::isGlobalFunction()
{
	return !is_method && is_global;
}

bool aeNodeFunction::isStaticMethod()
{
	return is_method && is_static;
}

bool aeNodeFunction::isAnonymousFunction()
{
	return is_anon;
}

std::string aeNodeFunction::printtext()
{
	std::string s1 = std::string("Fn ") + m_name + "(";
	if (m_parameters.size() > 0)
	{
		for (std::size_t i = 0; i < m_parameters.size(); ++i)
		{
			s1 += static_cast <aeNodeVarRef*>(m_parameters[i])->TypeString;
			if (i < m_parameters.size() - 1)
				s1 += ",";
		}
	}
	s1 += ")";
	return s1;
}

//////////////////////////////////////////////////////////////////////////

aeNodeWhile::aeNodeWhile()
{
	m_type = WhileLoop;

	doWhile = false;

	block = new aeNodeBlock();
	add(block);
}

std::string aeNodeWhile::printtext()
{
	return std::string("While");
}

//////////////////////////////////////////////////////////////////////////

aeNodeFor::aeNodeFor()
{
	m_type = ForLoop;

	block = new aeNodeBlock();
	add(block);
}

std::string aeNodeFor::printtext()
{
	return std::string("For ") + expr->printtext() + ";" + incrExpr->printtext();
}

//////////////////////////////////////////////////////////////////////////

aeNodeUsing::aeNodeUsing()
{
	m_type = AEN_USING;
}

std::string aeNodeUsing::printtext()
{
	return std::string("Using ") + arg->exprstr();
}


//////////////////////////////////////////////////////////////////////////

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