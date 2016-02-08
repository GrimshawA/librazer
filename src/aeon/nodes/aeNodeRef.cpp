#include "aeNodeRef.h"
#include "../aeon_compiler.h"

aeNodeRef::aeNodeRef()
{
	m_nodeType = AEN_REF;
}

aeQualType aeNodeRef::getQualifiedType(aeon_compiler* c)
{
	aeQualType qt = c->getVariable(m_name).type;
	return qt;
}

std::string aeNodeRef::str() const
{
	return m_name;
}