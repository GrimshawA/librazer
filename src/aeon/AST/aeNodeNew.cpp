#include "aeNodeNew.h"
#include <AEON/Compiler/aeCompiler.h>

aeNodeNew::aeNodeNew()
{
	m_nodeType = AEN_NEW;
}

aeQualType aeNodeNew::getQualifiedType(AECompiler* c)
{
	return m_instanceType;
}