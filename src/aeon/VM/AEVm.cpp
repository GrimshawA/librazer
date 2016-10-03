#include <AEON/VM/AEVm.h>
#include <AEON/Runtime/aeByteCode.h>
#include <AEON/AEContext.h>
#include <AEON/Runtime/AEObject.h>
#include <AEON/Runtime/AEGeneric.h>
#include <AEON/DebugDefs.h>

#include <AEON/VM/AEVmCalls.h>

#define vm_start(x) case x:{
#define vm_end break;}

#define LOGVM(x, a, b, c) printf("[vm] %s %d %d %d\n", #x, a, b, c);

#define VMLOGLEVEL 5
#define vm_log(tag, level, STR) if(level > VMLOGLEVEL) printf("%s: %s\n", tag, STR);

#include "VmInstructions.h"

AEVirtualMachine::AEVirtualMachine()
{
	memset(m_stk.stack.data(), 0, m_stk.stack.size());
}

AEVirtualMachine::AEVirtualMachine(AEContext* context)
{
	m_ctx = context;
}

AEValue AEVirtualMachine::call(AEValue obj, const std::string& functionName, AEValueList args)
{
	AEValue returnValue;

	return returnValue;
}

AEModule* AEVirtualMachine::get_current_mod()
{
	return m_stk.frames[m_stk.frames.size() - 1].module;
}

void AEVirtualMachine::callMethod(AEObject* object, const std::string& prototype)
{
	m_stk.ebp = m_stk.esp;

	m_stk.pushThisPtr(object->m_obj);
	  
	//printf("callMethod: %s. pushing this, pointing to %x popped\n", prototype.c_str(), object->addr);

	call(*object->getType()->getModule(), prototype.c_str());
} 

void AEVirtualMachine::callMethod(AEObject* object, uint32_t methodId)
{

}

void AEVirtualMachine::prepare(aeFunctionId function)
{

}

void AEVirtualMachine::pushThis(void* obj)
{
	m_stk.push_addr(obj);
}

void AEVirtualMachine::setContext(AEContext* context)
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

void AEVirtualMachine::call(AEModule& module, const char* func)
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

int AEVirtualMachine::call(AEFunction* fn)
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
		argsMem.push_back(m_stk.popMemory(fn->params[i].getSize()));
	}

	// Prepare functions and call

	aeStackFrame callinfo;
	callinfo.name = fn->getName();
	callinfo.object = nullptr;
	callinfo.pc = fn->m_offset;
	callinfo.module = fn->m_module;
	callinfo.ebp = m_stk.esp;
	callinfo.function = fn;
	m_stk.frames.push_back(callinfo);

	m_stk.ebp = m_stk.esp;

	m_stk.alloc(fn->returnValueSize);

	for (int i = argsMem.size() - 1; i >= 0; --i)
	{
		m_stk.pushMemory(argsMem[i]);
	}

	// Launch the thread from this entry point
	execute(m_stk);

	return 0;
}

#include "VmDispatch.h"