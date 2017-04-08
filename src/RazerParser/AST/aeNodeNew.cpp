#include "aeNodeNew.h"
#include <RazerCompiler/RzCompiler.h>

aeNodeNew::aeNodeNew()
{
	m_nodeType = AEN_NEW;
}

aeQualType aeNodeNew::getQualifiedType(RzCompiler* c)
{
	m_instanceType = aeQualType(c->m_env->getTypeInfo(type));
    m_instanceType.m_typeString = type;
	return m_instanceType;
}
