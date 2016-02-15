#include "aeNodeNew.h"
#include <AEON/Compiler/aeCompiler.h>

aeQualType aeNodeNew::getQualifiedType(aeCompiler* c)
{
	return m_instanceType;
}