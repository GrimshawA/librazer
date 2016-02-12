#include "aeNodeNew.h"
#include "../aeon_compiler.h"

aeQualType aeNodeNew::getQualifiedType(aeCompiler* c)
{
	return m_instanceType;
}