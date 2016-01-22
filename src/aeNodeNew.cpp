#include "aeNodeNew.h"
#include "aeon_compiler.h"

aeQualType aeNodeVarRef::getQualifiedType(aeon_compiler* c)
{
	aeon_type* t = c->getVariable(m_name).type;
	aeQualType qt;
	qt.m_type = t;
	return qt;
}