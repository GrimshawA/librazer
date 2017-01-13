#ifndef RZTHREADCONTEXT_H__
#define RZTHREADCONTEXT_H__

#include <RazerVM/StackFrame.h>
#include <RazerVM/StackValue.h>

#include <Rzr/RzValue.h>
#include <RazerRuntime/AEObject.h>
#include <vector>
#include <stdint.h>

class AEFunction;
class aeVM;
class RzModule;

/**
	\struct RzThreadContext
	\brief Provides environment for each thread of execution in the vm

	This is also the entry point for starting to run code until the call stack collapses.

	Threads have their own unique state, including the program counter, the entire call stack, the stack for operations and so on.
	Advanced debugging is done by pausing execution and inspecting the thread state.
*/
class RzThreadContext
{
public:
	std::vector<unsigned char>  stack;
	std::vector<RzStackFrame>   frames;
	unsigned char*              esp;
	unsigned char*              ebp;
	int                         pc;
	RzStackFrame*               cl;
	RzEngine*                   engine;

public:

	RzThreadContext()
	{
		stack.resize(512000);
		esp = &stack[512000 - 1];
		ebp = esp;
	}

	RzModule& getModule();

	void pushVariant(const RzValue& v);
	void popVariant(RzValue& v);

	void pushVariantRef(const AEValueRef& ref);
	AEValueRef popVariantRef();

	void pushMemory(const std::vector<uint8_t>& mem);
	std::vector<uint8_t> popMemory(int size);

	void pushObject(AEObject* obj);

	void alloc(int size);

	uint64_t size()
	{
		return stack.size();
	}

	void print_registers()
	{
		printf("[esp] %x : index %d\n", esp, stack.data() + stack.size() - esp);
		printf("[ebp] %x : index %d\n", ebp, stack.data() + stack.size() - ebp);
	}

	void push_value(RzStackValue v)
	{
		esp -= sizeof(v);
		*reinterpret_cast<RzStackValue*>(esp) = v;
	}

	RzStackValue pop_value()
	{
		RzStackValue v;
		v = *reinterpret_cast<RzStackValue*>(esp);
		esp += sizeof(RzStackValue);

		return v;
	}

	RzStackValue getThisPtr()
	{
		RzStackValue thisvm;
		thisvm.ptr = reinterpret_cast<RzStackValue*>(ebp - int32_t(sizeof(RzStackValue)))->ptr;
		//thisvm.u64 -= sizeof(vm_value);
		//thisvm = *(vm_value*)(ebp - sizeof(vm_value));
		//printf("thisptr is %x from %x - 8\n", thisvm.ptr, ebp);
		return thisvm;
	}

	void pushThisPtr(void* ptr)
	{
		RzStackValue v;
		v.ptr = ptr;
		push_value(v);

		//printf("pushed this %x, read %x\n", ptr, getThisPtr().ptr);
	}

	void push_addr(void* ptr)
	{
		RzStackValue v;
		v.ptr = ptr;
		push_value(v);
	}

	void* pop_addr()
	{
		esp += sizeof(void*);
		return reinterpret_cast<void*>(esp - sizeof(void*));
	}

	void push_int32(int32_t v)
	{
		esp -= sizeof(int32_t);
		*reinterpret_cast<int32_t*>(esp) = v;
	}

	int32_t pop_int32()
	{
		int32_t rv = *reinterpret_cast<int32_t*>(esp);
		esp += sizeof(int32_t);
		return rv;
	}
};
#endif // RZTHREADCONTEXT_H__
