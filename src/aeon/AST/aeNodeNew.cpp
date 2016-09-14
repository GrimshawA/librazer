#include "aeNodeNew.h"
#include <AEON/Compiler/aeCompiler.h>

aeQualType aeNodeNew::getQualifiedType(AECompiler* c)
{
	return m_instanceType;
}