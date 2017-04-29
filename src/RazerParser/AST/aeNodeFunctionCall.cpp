#include "aeNodeFunctionCall.h"
#include "aeNodeFunction.h"
#include "AEStructNode.h"
#include <RazerCompiler/RzCompiler.h>

aeNodeFunctionCall::aeNodeFunctionCall()
: aeNodeExpr()
, m_function(nullptr)
, m_fn(nullptr)
{
	m_nodeType = AEN_FUNCTIONCALL;
}

uint32_t aeNodeFunctionCall::getReturnTypeSize()
{
	if (m_function)
	{
		return m_function->getReturnType().getSize();
	}
}

RzQualType aeNodeFunctionCall::getArgType(uint32_t index)
{
	if (m_function)
	{
		return m_function->getParameterType(index);
	}
	return RzQualType();
}

bool aeNodeFunctionCall::hasLinkedFunction()
{
	return (m_function);
}

std::string aeNodeFunctionCall::str() const
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
	for (std::size_t i = 0; i < m_args.size(); ++i)
	{
			s1 += m_args[i]->str();
			if (i < m_args.size() - 1)
				s1 += ",";
	}
	s1 += ")";

	return s1;
}
