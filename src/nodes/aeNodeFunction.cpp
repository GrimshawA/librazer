#include "aeNodeFunction.h"
#include "aeNodeExpr.h"
#include "aeNodeBlock.h"

aeNodeFunction::aeNodeFunction()
{
	m_nodeType = AEN_FUNCTION;

	m_block.reset(new aeNodeBlock());
	//add(m_block);

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
	/*if (m_parameters.size() > 0)
	{
		for (std::size_t i = 0; i < m_parameters.size(); ++i)
		{
			s1 += static_cast <aeNodeVarRef*>(m_parameters[i])->TypeString;
			if (i < m_parameters.size() - 1)
				s1 += ",";
		}
	}
	s1 += ")";*/
	return s1;
}
