#ifndef aeon_vm_h__
#define aeon_vm_h__

#include "aeon_module.h"
#include "aeon_generics.h"
#include <vector>
#include <stdint.h>

class aeon_vm;
class aeon_context;

struct StackFrameInfo
{
	int32_t base;
	std::string name;
	uint32_t pc;
	aeon_module* module;

	/// If the function being called is a method, object contains the address of the object its called on
	atom_objectref object;
};

struct ExecutionContext
{
	std::vector<unsigned char>  stack;
	std::vector<StackFrameInfo> frames;
	unsigned char*              sp;
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
		uint64_t                   eax; // accumulator, 64 bits to be able to hold function return absolute addresses
		uint64_t                   ebp; // base pointer, just an absolute address of our base pointer
		uint64_t                   esp; // stack pointer, absolute address of the stack "top"
		std::vector<unsigned char> stack;
		unsigned char*             sp;
		std::vector<StackFrameInfo> callstack;

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
