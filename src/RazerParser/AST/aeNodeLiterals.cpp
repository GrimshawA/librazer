#include "aeNodeLiterals.h"
#include <RazerCompiler/RzCompiler.h>

RzQualType aeNodeString::getQualifiedType(RzCompiler* c, RzQualType base) {
	return RzQualType(c->m_env->getTypeInfo("string"));
}

RzQualType aeNodeFloat::getQualifiedType(RzCompiler* c, RzQualType base) {
	return RzQualType(c->m_env->getTypeInfo("float"));
}

RzQualType aeNodeInteger::getQualifiedType(RzCompiler* c, RzQualType base) {
	return RzQualType(c->m_env->getTypeInfo("int32"));
}
