#include <RazerVM/AEVmThread.h>

void AEVmThread::start()
{
	m_thread = std::thread(&AEVmThread::threadFunc, this);
}

uint64_t AEVmThread::stackSize()
{
	return stack.size();
}

void AEVmThread::threadFunc()
{
	// Threads launched by the vm, will be tied to the execution of this function.
	// Once this function terminates, the thread has exited and the vm will handle the proper signaling

	/*aeStackFrame callinfo;
	callinfo.name = fn->getName();
	callinfo.object = nullptr;
	callinfo.pc = fn->offset;
	callinfo.module = fn->m_module;
	callinfo.ebp = m_stk.esp;
	callinfo.function = fn;
	m_stk.frames.push_back(callinfo);*/
}