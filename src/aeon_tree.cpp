#include "aeon_tree.h"
#include "aeon_expression.h"

ast_ifbranch::ast_ifbranch()
{
	type = IfBranch;

	// we have to have an expression and block so..
	block = new ast_codeblock();
	add(block);
}

std::string ast_ifbranch::printtext()
{
	return "if " + expr->exprstr();
}

//////////////////////////////////////////////////////////////////////////

aeon_ast_function::aeon_ast_function()
{
	type = FuncDecl;

	m_block = new ast_codeblock();
	add(m_block);

	is_global = false;
	is_method = false;
	is_destructor = false;
	is_constructor = false;
}

std::string aeon_ast_function::printtext()
{
	std::string s1 = std::string("Fn ") + m_name + "(";
	if (m_parameters.size() > 0)
	{
		for (std::size_t i = 0; i < m_parameters.size(); ++i)
		{
			s1 += static_cast <ast_varexpr*>(m_parameters[i])->TypeString;
			if (i < m_parameters.size() - 1)
				s1 += ",";
		}
	}
	s1 += ")";
	return s1;
}

//////////////////////////////////////////////////////////////////////////

ast_while::ast_while()
{
	type = WhileLoop;

	doWhile = false;

	block = new ast_codeblock();
	add(block);
}

std::string ast_while::printtext()
{
	return std::string("While");
}

//////////////////////////////////////////////////////////////////////////

ast_for::ast_for()
{
	type = ForLoop;

	block = new ast_codeblock();
	add(block);
}

std::string ast_for::printtext()
{
	return std::string("For ") + expr->printtext() + ";" + incrExpr->printtext();
}

//////////////////////////////////////////////////////////////////////////

ast_using::ast_using()
{
	type = Using;
}

std::string ast_using::printtext()
{
	return std::string("Using ") + arg->exprstr();
}