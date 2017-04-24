#include "aeNodeNew.h"
#include <RazerCompiler/RzCompiler.h>

aeNodeNew::aeNodeNew()
{
	m_nodeType = AEN_NEW;
}

RzQualType aeNodeNew::getQualifiedType(RzCompiler* c, RzQualType base) {
	m_instanceType = RzQualType(c->m_env->getTypeInfo(type));
    m_instanceType.m_typeString = type;
	return m_instanceType;
}
