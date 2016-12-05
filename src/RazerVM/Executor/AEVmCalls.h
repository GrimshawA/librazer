#ifndef AEVmCalls_h__
#define AEVmCalls_h__

static inline void DoCall(RzVirtualMachine* vm, int functionIndex)
{
	AEFunction* functionData = vm->m_ctx->m_functionTable[functionIndex];

#if defined TRACE_VM
	printf("Calling function %s\n", functionData->getName().c_str());
	functionData->printByteCode();
#endif

	RzStackFrame callinfo;
	callinfo.name = "unknown";
	callinfo.pc = functionData->m_offset - 1;
	callinfo.module = functionData->m_module;
	callinfo.ebp = vm->m_mainContext.ebp;
	callinfo.function = functionData;
	vm->m_mainContext.frames.push_back(callinfo);
	vm->m_mainContext.cl = &vm->m_mainContext.frames[vm->m_mainContext.frames.size() - 1];
}

static inline void DoDynamicCall(RzVirtualMachine* vm, int functionIndex)
{
	//AEValue v = vm->m_stk.popVar();
	
}

#endif // AEVmCalls_h__
