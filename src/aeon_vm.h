#ifndef aeon_vm_h__
#define aeon_vm_h__

#include "aeon_module.h"
#include "aeon_generics.h"
#include <vector>
#include <stdint.h>

class aeon_vm;
class aeon_context;
class aeon_object;

struct StackFrameInfo
{
	int32_t base;
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
*/
struct ExecutionContext
{
	std::vector<unsigned char>  stack;
	std::vector<StackFrameInfo> frames;
	unsigned char*              esp;
	unsigned char*              ebp;
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

		void execute();

		int getStackIndex();

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

		atom_objectref pop_objectref();
};

#endif // aeon_vm_h__
