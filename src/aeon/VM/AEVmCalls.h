#ifndef AEVmCalls_h__
#define AEVmCalls_h__

static inline void DoCall(AEVirtualMachine* vm, int functionIndex)
{
	AEFunction* functionData = vm->m_ctx->m_functionTable[functionIndex];

	aeStackFrame callinfo;
	callinfo.name = "unknown";
	callinfo.pc = functionData->m_offset - 1;
	callinfo.module = functionData->m_module;
	callinfo.ebp = vm->m_stk.ebp;
	callinfo.function = functionData;
	vm->m_stk.frames.push_back(callinfo);
	vm->m_stk.cl = &vm->m_stk.frames[vm->m_stk.frames.size() - 1];
}

static inline void DoDynamicCall(AEVirtualMachine* vm, int functionIndex)
{
	//AEValue v = vm->m_stk.popVar();
	
}

#endif // AEVmCalls_h__
