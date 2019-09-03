#include <razer/frontend/AST/aeNodeIdentifier.h>
#include <razer/frontend/compiler/RzCompiler.h>

aeNodeIdentifier::aeNodeIdentifier()
{
	m_nodeType = AEN_IDENTIFIER;
}

aeNodeIdentifier::aeNodeIdentifier(const std::string& name)
{
    m_nodeType = AEN_IDENTIFIER;
    m_name = name;
}

std::string aeNodeIdentifier::str() const
{
	return m_name;
}


std::string RzConstructExpr::str() const
{
    return "ConstructExpr" + base->str() + "<" + param->str() + ">";
}
