#include "aeNodeLiterals.h"
#include <aeon/aeon_compiler.h>

aeQualType aeNodeString::getQualifiedType(aeCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("string"));
}

aeQualType aeNodeFloat::getQualifiedType(aeCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("float"));
}