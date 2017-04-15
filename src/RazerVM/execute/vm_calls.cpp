#include <Logger.h>
#include <RazerVM/ThreadContext.h>
#include <Rzr/RzEngine.h>

static inline void DoCall(RzThreadContext& ctx, int functionIndex)
{
	AEFunction* functionData = ctx.engine->m_functionTable[functionIndex];

#if defined TRACE_VM
	RZLOG("Calling function %s\n", functionData->getName().c_str());
	functionData->printByteCode();
#endif

	RzStackFrame callinfo;
	callinfo.name = "unknown";
	callinfo.pc = functionData->m_offset - 1;
	callinfo.module = functionData->m_module;
	callinfo.ebp = ctx.ebp;
	callinfo.function = functionData;
	ctx.frames.push_back(callinfo);
	ctx.cl = &ctx.frames[ctx.frames.size() - 1];
}

static inline void DoDynamicCall(RzThreadContext& ctx, int functionIndex)
{
	//AEValue v = vm->m_stk.popVar();
	
}

static inline void DoNativeCall(RzThreadContext& ctx, int moduleIndex, int functionIndex) {
    RzGeneric g; g.m_threadCtx = &ctx;
    ctx.engine->modules[moduleIndex]->m_nativeFunctions[functionIndex].f(g);
}
