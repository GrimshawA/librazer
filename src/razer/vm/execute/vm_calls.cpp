#include <razer/utils/Logger.h>
#include <razer/vm/ThreadContext.h>
#include <razer/runtime/RzEngine.h>

#include <assert.h>

static inline void DoCall(RzThreadContext& ctx, int moduleIndex, int functionIndex, int other)
{
    assert(functionIndex >= 0);

    RzModule* m = ctx.engine->modules[moduleIndex].get();
    RzFunction* functionData = &m->m_functions[functionIndex];

	RzStackFrame callinfo;
	callinfo.name = functionData->getName();
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

static inline void DoNativeCall(RzThreadContext& cx, int moduleIndex, int functionIndex, int type) {
    RzGeneric g;
    g.m_threadCtx = &cx;

    // Fucked up on uint8 instruction operand..
    assert(functionIndex <= 255);

    auto* mod = cx.engine->modules[moduleIndex].get();
    auto* func = &mod->m_functions[functionIndex];

    if (func->m_isconstructor)
    {
        // constructor
        auto* mem = cx.pop_value().ptr;
        mod->m_functions[functionIndex].constructorCallback(mem, nullptr);
    }
    else
    {
        // method
        mod->m_functions[functionIndex].methodCallback(g);
    }
}
