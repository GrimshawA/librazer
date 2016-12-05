#include <RazerVM/VirtualMachine.h>
#include <RazerVM/InstructionSet.h>
#include <Rzr/RzEngine.h>
#include <AEON/Runtime/AEObject.h>
#include <AEON/Runtime/AEGeneric.h>
#include <AEON/DebugDefs.h>

#include <RazerVM/Executor/AEVmCalls.h>

#define vm_start(x) case x:{
#define vm_end break;}

#define LOGVM(x, a, b, c) printf("[vm] %s %d %d %d\n", #x, a, b, c);

#define VMLOGLEVEL 5
#define vm_log(tag, level, STR) if(level > VMLOGLEVEL) printf("%s: %s\n", tag, STR);

#include <RazerVM/Executor/VmInstructions.h>

// [API]

RzVirtualMachine::RzVirtualMachine()
{
	memset(m_mainContext.stack.data(), 0, m_mainContext.stack.size());
}

RzVirtualMachine::RzVirtualMachine(RzEngine* context)
{
	m_ctx = context;
}

void RzVirtualMachine::execute(int functionId)
{

}

void RzVirtualMachine::executeAsync(int functionId)
{

}

// End of [API]


RzValue RzVirtualMachine::call(RzValue obj, const std::string& functionName, AEValueList args)
{
	RzValue returnValue;

	return returnValue;
}

RzModule* RzVirtualMachine::get_current_mod()
{
	return m_mainContext.frames[m_mainContext.frames.size() - 1].module;
}

void RzVirtualMachine::callMethod(AEObject* object, const std::string& prototype)
{
	m_mainContext.ebp = m_mainContext.esp;

	m_mainContext.pushThisPtr(object->m_obj);
	  
	//printf("callMethod: %s. pushing this, pointing to %x popped\n", prototype.c_str(), object->addr);

	call(*object->getType()->getModule(), prototype.c_str());
} 

void RzVirtualMachine::callMethod(AEObject* object, uint32_t methodId)
{

}

void RzVirtualMachine::prepare(aeFunctionId function)
{

}

void RzVirtualMachine::pushThis(void* obj)
{
	m_mainContext.push_addr(obj);
}

void RzVirtualMachine::setContext(RzEngine* context)
{
	m_ctx = context;
} 

void printBits2(size_t const size, void const * const ptr)
{
		unsigned char *b = (unsigned char*)ptr;
		unsigned char byte;
		int i, j;

		for (i = size - 1; i >= 0; i--)
		{
			for (j = 7; j >= 0; j--)
			{
				byte = b[i] & (1 << j);
				byte >>= j;
				printf("%u", byte);
			}
		}
		puts("");
}

void RzVirtualMachine::call(RzModule& module, const char* func)
{
	aeFunctionId functionId = 0;

	AEFunction* function = m_ctx->getFunctionByName(func);
	int i = 0;

	if (function)
	{
		call(function);
	}
	else
	{
		printf("Couldn't find the calling function\n");
	}
}

int RzVirtualMachine::call(AEFunction* fn)
{
	if (!fn->m_compiled)
	{
		//printf("This function is not compiled '%s'.\n", function->getSymbolName().c_str());
		return 1;
	}

	prepare(fn->id);

	// Get arguments
	printf("Calling script func with %d args\n", fn->params.size());


	std::vector<std::vector<uint8_t>> argsMem;
	for (int i = 0; i < fn->params.size(); ++i)
	{		
		printf("pop memory for param %d\n", fn->params[i].getSize());
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
		m_mainContext.pushMemory(argsMem[i]);
	}

	// Launch the thread from this entry point
	execute(m_mainContext);

	return 0;
}

#include <RazerVM/Executor/VmDispatch.h>