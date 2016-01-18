#ifndef aeon_vm_h__
#define aeon_vm_h__

#include "aeon_module.h"
#include "aeon_generics.h"
#include <vector>
#include <stdint.h>

#include "aeon_context.h"

class aeon_vm;
class aeon_object;

/**
	When pushing operands to the stack they
	must be properly aligned and padded.
	This struct helps with this, making the code more
	portable and easier to handle.
*/
struct vm_value
{
	union
	{
		uint8_t  u8;
		int8_t   i8;
		uint16_t u16;
		int16_t  i16;
		uint32_t u32;
		int32_t  i32;
		uint64_t u64;
		int64_t  i64;
		float    fp;
		float    dp;
		void*    ptr;
	};

	static vm_value make_int32(int32_t v)
	{
		vm_value vmv; vmv.i32 = v; return vmv;
	}

	static vm_value make_uint32(uint32_t v)
	{
		vm_value vmv; vmv.u32 = v; return vmv;
	}

	static vm_value make_float(float v)
	{
		vm_value vmv; vmv.fp = v; return vmv;
	}

	static vm_value make_double(double v)
	{
		vm_value vmv; vmv.dp = v; return vmv;
	}

	static vm_value make_ptr(void* v)
	{
		vm_value vmv; vmv.ptr = v; return vmv;
	}
};

struct StackFrameInfo
{
	std::string name;
	uint32_t pc;               ///< So we can restore the program counter of the caller
	unsigned char* ebp;        ///< So we can restore the base pointer of the caller
	aeon_module* module;

	/// If the function being called is a method, object contains the address of the object its called on
	atom_objectref object;
};

/**
	\struct ExecutionContext
	\brief Each thread of execution gets its own context information

	Each thread needs its own stack to perform operations in parallel,
	its very own stack of calls and registers for the utility.

	Types pushable:
	ptr
	int32
*/
struct ExecutionContext
{
	std::vector<unsigned char>  stack;
	std::vector<StackFrameInfo> frames;
	unsigned char*              esp;
	unsigned char*              ebp;

	ExecutionContext()
	{
		stack.resize(512000);
		esp = &stack[512000 - 1];
		ebp = esp;
	}

	void print_registers()
	{
		printf("[esp] %x : index %d\n", esp, stack.data() + stack.size() - esp);
		printf("[ebp] %x : index %d\n", ebp, stack.data() + stack.size() - ebp);
	}

	void push_value(vm_value v)
	{
		esp -= sizeof(v);
		*reinterpret_cast<vm_value*>(esp) = v;
	}

	vm_value pop_value()
	{
		vm_value v;
		v = *reinterpret_cast<vm_value*>(esp);
		esp += sizeof(vm_value);
		return v;
	}

	vm_value getThisPtr()
	{
		vm_value thisvm;
		thisvm.ptr = reinterpret_cast<vm_value*>(ebp - int32_t(sizeof(vm_value)))->ptr;
		//thisvm.u64 -= sizeof(vm_value);
		//thisvm = *(vm_value*)(ebp - sizeof(vm_value));
		printf("thisptr is %x from %x - 8\n", thisvm.ptr, ebp);
		return thisvm;
	}

	void pushThisPtr(void* ptr)
	{
		vm_value v;
		v.ptr = ptr;
		push_value(v);

		printf("pushed this %x, read %x\n", ptr, getThisPtr().ptr);
	}

	void push_addr(void* ptr)
	{
		esp -= sizeof(ptr);
		memcpy(esp, ptr, sizeof(ptr));
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

/**
	\class aeon_vm
	\brief The vm is in charge of executing the program in its byte code form

	Its job is simply to take a starting member function or global, and execute it 
	and its nested calls until it ends.
*/
class aeon_vm
{
	public:

		aeon_context*              ctx;		
		ExecutionContext           m_stk;

		typedef unsigned char* SFLocalVar;

		/// Every global and local variable gets its own entry here
		/// The vm uses these to do memory management
		std::vector<SFLocalVar> localsStack;

	public:

		aeon_vm();

		aeon_module* get_current_mod();

		void setContext(aeon_context* context);

		void prepare(aeFunctionId function);
		void pushThis(void* obj);
		void execute();

		/// Call a method on the given script object by its name
		void callMethod(aeon_object* object, const std::string& prototype);

		/// Call a method on the given script object by the method id
		void callMethod(aeon_object* object, uint32_t methodId);

		/// Call a script function
		void call(aeon_module& module, const char* func);

		void setArgFloat(uint32_t index, float v);

		/// Set any arg directly from a variant
		void setArg(uint32_t index, aeon_variant v);

		aeon_variant getArg(uint32_t index);

		float getArgFloat(uint32_t index);

		void push(uint64_t n);

		/// Pushes to stack N bytes to fit something
		void push_bytes(uint32_t bytes);

		/// Push a float value to stack
		void push_float(float value);

		/// Push a double to stack
		void push_double(double value);

		/// Push a int32 value to stack
		void push_int32(int32_t value);

		/// Pushes an object ref to the stack
		void push_objectref(atom_objectref ref);

		uint64_t pop();

		float pop_float();

		double pop_double();

		int32_t pop_int32();
};

#endif // aeon_vm_h__
