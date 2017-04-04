#include "aeNodeLiterals.h"
#include <RazerCompiler/aeCompiler.h>

aeQualType aeNodeString::getQualifiedType(RzCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("string"));
}

aeQualType aeNodeFloat::getQualifiedType(RzCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("float"));
}

aeQualType aeNodeInteger::getQualifiedType(RzCompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("int32"));
}