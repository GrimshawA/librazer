#include <razer/vm/VirtualMachine.h>
#include <razer/vm/ExecDispatch.h>
#include <razer/vm/InstructionSet.h>

#include <RazerRuntime/RzEngine.h>
#include <RazerRuntime/AEObject.h>
#include <RazerRuntime/Generic.h>
#include <DebugDefs.h>
#include <Logger.h>

#include <cstring>

RzVirtualMachine::RzVirtualMachine(RzEngine& context)
    : m_ctx(context)
{
	memset(m_mainContext.stack.data(), 0, m_mainContext.stack.size());
    m_mainContext.engine = &m_ctx;
}

void RzVirtualMachine::execute(int functionId) {

}

void RzVirtualMachine::executeAsync(int functionId) {

}

RzValue RzVirtualMachine::call(RzValue obj, const std::string& functionName, AEValueList args) {
	RzValue returnValue;

	return returnValue;
}

void RzVirtualMachine::callMethod(AEObject* object, const std::string& prototype) {
	m_mainContext.ebp = m_mainContext.esp;

    //call(*object->getType()->getModule(), prototype.c_str());
}

void RzVirtualMachine::callMethod(AEObject* object, uint32_t methodId) {

}

void RzVirtualMachine::prepare(aeFunctionId function) {

}

void RzVirtualMachine::pushThis(void* obj) {
	m_mainContext.push_addr(obj);
}

RzEngine& RzVirtualMachine::getContext() const
{
    return m_ctx;
}

RzThreadContext& RzVirtualMachine::getMainThread()
{
    return m_mainContext;
}

void printBits2(size_t const size, void const * const ptr) {
		unsigned char *b = (unsigned char*)ptr;
		unsigned char byte;
		int i, j;

		for (i = size - 1; i >= 0; i--)
		{
			for (j = 7; j >= 0; j--)
			{
				byte = b[i] & (1 << j);
				byte >>= j;
				RZLOG("%u", byte);
			}
		}
		puts("");
}

void RzVirtualMachine::call(const char* func) {
	aeFunctionId functionId = 0;

    // Find the function in any module from its fully qualified name
    // package.namespace.class.method
    RzFunction* function = m_ctx.getFunctionByName(func);
	int i = 0;

	if (function)
	{
		call(function);
	}
	else
	{
		RZLOG("Couldn't find the calling function\n");
    }
}

int RzVirtualMachine::call(RzFunction* fn) {
	if (!fn->m_compiled)
	{
		return 1;
	}

	prepare(fn->id);

	// Get arguments
	RZLOG("Calling script func with %d args\n", fn->params.size());


	std::vector<std::vector<uint8_t>> argsMem;
	for (int i = 0; i < fn->params.size(); ++i)
	{		
		RZLOG("pop memory for param %d\n", fn->params[i].getSize());
		argsMem.push_back(m_mainContext.popMemory(fn->params[i].getSize()));
	}

	// Prepare functions and call

	RzStackFrame callinfo;
	callinfo.name = fn->getName();
	callinfo.object = nullptr;
	callinfo.pc = fn->m_offset;
	callinfo.module = fn->m_module;
	callinfo.ebp = m_mainContext.esp;
	callinfo.function = fn;
	m_mainContext.frames.push_back(callinfo);

	m_mainContext.ebp = m_mainContext.esp;

	m_mainContext.alloc(fn->returnValueSize);

	for (int i = argsMem.size() - 1; i >= 0; --i)
	{
        void* ptr = reinterpret_cast<void*>(argsMem[i].data());
		m_mainContext.pushMemory(argsMem[i]);
	}

	// Launch the thread from this entry point
	dispatch_execute(m_mainContext);

	return 0;
}
