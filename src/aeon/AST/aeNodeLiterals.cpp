#include "aeNodeLiterals.h"
#include <AEON/Compiler/aeCompiler.h>

aeQualType aeNodeString::getQualifiedType(AECompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("string"));
}

aeQualType aeNodeFloat::getQualifiedType(AECompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("float"));
}

aeQualType aeNodeInteger::getQualifiedType(AECompiler* c)
{
	return aeQualType(c->m_env->getTypeInfo("int32"));
}