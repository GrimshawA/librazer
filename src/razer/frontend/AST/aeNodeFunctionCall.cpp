#include "aeNodeFunctionCall.h"
#include "aeNodeFunction.h"
#include "AEStructNode.h"
#include <razer/frontend/compiler/RzCompiler.h>

RzCallNode::RzCallNode()
: aeNodeExpr()
, m_function(nullptr)
, m_fn(nullptr)
{
	m_nodeType = AEN_FUNCTIONCALL;
}

RzFunction* RzCallNode::getFunction()
{
    return m_fn;
}

uint32_t RzCallNode::getReturnTypeSize()
{
	if (m_function)
	{
		return m_function->getReturnType().getSize();
	}
}

RzQualType RzCallNode::getArgType(uint32_t index)
{
	if (m_function)
	{
		return m_function->getParameterType(index);
	}
	return RzQualType();
}

bool RzCallNode::hasLinkedFunction()
{
	return (m_function);
}

std::string RzCallNode::str() const
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

RzLambdaExprNode::RzLambdaExprNode(aeNodeExpr* arg, aeNodeBlock* blk)
{
    this->arg = arg;
    this->block = blk;
}
