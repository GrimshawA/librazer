#include <razer/frontend/AST/aeNodeIdentifier.h>
#include <RazerCompiler/RzCompiler.h>

aeNodeIdentifier::aeNodeIdentifier()
{
	m_nodeType = AEN_IDENTIFIER;
}

aeNodeIdentifier::aeNodeIdentifier(const std::string& name)
{
    m_nodeType = AEN_IDENTIFIER;
    m_name = name;
}

IRValue* aeNodeIdentifier::emitIR(IRBuilder& builder)
{
    auto* val = builder.makeValue();
    val->name = m_name;
    return val;
}

std::string aeNodeIdentifier::str() const
{
	return m_name;
}


std::string RzConstructExpr::str() const
{
    return "ConstructExpr" + base->str() + "<" + param->str() + ">";
}
