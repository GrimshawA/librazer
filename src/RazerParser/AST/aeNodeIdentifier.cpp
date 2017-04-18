#include <RazerParser/AST/aeNodeIdentifier.h>
#include <RazerCompiler/RzCompiler.h>

aeNodeIdentifier::aeNodeIdentifier()
{
	m_nodeType = AEN_IDENTIFIER;
}

RzQualType aeNodeIdentifier::getQualifiedType(RzCompiler* c)
{
	RzType* typeInfo = c->m_env->getTypeInfo(m_name);
	if (typeInfo)
	{
		// This identifier is actually a type (enum, class)
		RzQualType qt;
		qt.m_type = typeInfo;
		return qt;
	}

	RzQualType qt = c->getVariable(m_name).type;
    if (!qt.m_type) {
        qt.m_type = c->m_env->getTypeInfo(qt.m_typeString);
    }
	return qt;
}

std::string aeNodeIdentifier::str() const
{
	return m_name;
}
