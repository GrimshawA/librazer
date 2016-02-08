#include "aeNodeNew.h"
#include "../aeon_compiler.h"

aeQualType aeNodeNew::getQualifiedType(aeon_compiler* c)
{
	return m_instanceType;
}