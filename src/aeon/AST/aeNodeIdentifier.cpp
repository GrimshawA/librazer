#include <AEON/AST/aeNodeIdentifier.h>
#include <AEON/Compiler/aeCompiler.h>

aeNodeIdentifier::aeNodeIdentifier()
{
	m_nodeType = AEN_IDENTIFIER;
}

aeQualType aeNodeIdentifier::getQualifiedType(aeCompiler* c)
{
	aeType* typeInfo = c->m_env->getTypeInfo(m_name);
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