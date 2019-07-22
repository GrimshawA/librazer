#include "aeNodeNew.h"
#include <razer/frontend/compiler/RzCompiler.h>

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
