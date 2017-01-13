#include <RazerParser/AST/aeNodeIdentifier.h>
#include <RazerCompiler/aeCompiler.h>

aeNodeIdentifier::aeNodeIdentifier()
{
	m_nodeType = AEN_IDENTIFIER;
}

aeQualType aeNodeIdentifier::getQualifiedType(AECompiler* c)
{
	RzType* typeInfo = c->m_env->getTypeInfo(m_name);
	if (typeInfo)
	{
		// This identifier is actually a type (enum, class)
		aeQualType qt;
		qt.m_type = typeInfo;
		return qt;
	}

	aeQualType qt = c->getVariable(m_name).type;
	return qt;
}

std::string aeNodeIdentifier::str() const
{
	return m_name;
}