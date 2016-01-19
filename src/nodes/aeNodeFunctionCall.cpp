#include "aeNodeFunctionCall.h"
#include "aeNodeClass.h"

aeNodeFunctionCall::aeNodeFunctionCall()
{
	m_type = FuncCall;
}

std::string aeNodeFunctionCall::exprstr()
{
	std::string s1 = m_name;
	if (templateTypeArguments.size() > 0)
	{
		s1 += "<";
		for (std::size_t i = 0; i < templateTypeArguments.size(); ++i)
		{
			s1 += templateTypeArguments[i].TypeString;
			if (i < templateTypeArguments.size() - 1)
				s1 += ",";
		}
		s1 += ">";
	}
	s1 += "(";
	if (m_args.size() > 0)
	{
		for (std::size_t i = 0; i < m_args.size(); ++i)
		{
			s1 += m_args[i]->exprstr();
			if (i < m_args.size() - 1)
				s1 += ",";
		}
	}
	s1 += ")";

	if (m_items.size() > 0)
	{
		s1 += "." + static_cast<aeNodeExpr*>(m_items[0])->exprstr();
	}
	return s1;
}

std::string aeNodeFunctionCall::printtext()
{
	std::string s1 = std::string("Call ") + exprstr();
	return s1;
}