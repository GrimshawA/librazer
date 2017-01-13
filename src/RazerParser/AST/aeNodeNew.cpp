#include "aeNodeNew.h"
#include <RazerCompiler/aeCompiler.h>

aeNodeNew::aeNodeNew()
{
	m_nodeType = AEN_NEW;
}

aeQualType aeNodeNew::getQualifiedType(AECompiler* c)
{
	m_instanceType = aeQualType(c->m_env->getTypeInfo(type));
	return m_instanceType;
}