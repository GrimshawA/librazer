#include "aeNodeLiterals.h"
#include <AEON/Compiler/aeCompiler.h>

aeQualType aeNodeString::getQualifiedType(aeCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("string"));
}

aeQualType aeNodeFloat::getQualifiedType(aeCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("float"));
}

aeQualType aeNodeInteger::getQualifiedType(aeCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("int32"));
}