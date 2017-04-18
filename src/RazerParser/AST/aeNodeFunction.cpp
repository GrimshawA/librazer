#include "aeNodeFunction.h"
#include "aeNodeExpr.h"
#include "aeNodeBlock.h"
#include <RazerParser/AST/AEBaseNode.h>
#include "AEStructNode.h"

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

std::string aeNodeFunction::str() const
{
    std::string s = std::string("  ");
    s += m_block->str();
    s += "\n";
	return s;
}

RzQualType aeNodeFunction::getParameterType(uint32_t index)
{
	return m_paramTypes[index];
}

RzQualType aeNodeFunction::getReturnType()
{
	return m_returnType;
}

std::string aeNodeFunction::visibilityText()
{
	if (visibility == AEStructNode::VISIBILITY_PUBLIC)
		return "public";
	if (visibility == AEStructNode::VISIBILITY_PROTECTED)
		return "protected";
	if (visibility == AEStructNode::VISIBILITY_PRIVATE)
		return "private";

	return "undefined";
}
