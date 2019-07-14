#include "aeNodeNew.h"
#include <RazerCompiler/RzCompiler.h>

aeNodeNew::aeNodeNew()
{
	m_nodeType = AEN_NEW;
}

aeNodeNew::aeNodeNew(aeNodeExpr* expr)
{
    m_nodeType = AEN_NEW;
    newExpr = expr;
}

IRValue* aeNodeNew::emitIR(IRBuilder& builder)
{
    return builder.newObject();
}
