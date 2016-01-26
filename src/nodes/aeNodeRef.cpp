#include "aeNodeRef.h"
#include "../aeon_compiler.h"

aeNodeRef::aeNodeRef()
{
	m_nodeType = AEN_REF;
}

aeQualType aeNodeRef::getQualifiedType(aeon_compiler* c)
{
	aeon_type* t = c->getVariable(m_name).type;
	aeQualType qt;
	qt.m_type = t;
	return qt;
}

std::string aeNodeRef::str() const
{
	return m_name;
}