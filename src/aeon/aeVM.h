#ifndef aeon_vm_h__
#define aeon_vm_h__

#include <AEON/aeModule.h>
#include <AEON/aeThreadState.h>
#include <AEON/aeContext.h>

#include <vector>
#include <stdint.h>


class aeVM;
class aeon_object;

#define STATE_ASSERT (assert((esp > m_stack.data() + m_stack.size()); assert((esp < m_stack.data())); 

/**
	\class aeon_vm
	\brief The vm is in charge of executing the program in its byte code form

	Its job is simply to take a starting member function or global, and execute it 
	and its nested calls until it ends.
*/
class aeVM
{
	public:

		aeContext*           m_ctx;
		aeThreadState           m_stk;

	public:

		aeVM();

		aeVM(aeContext* context);

		aeon_module* get_current_mod();

		void setContext(aeContext* context);

		void prepare(aeFunctionId function);
		void pushThis(void* obj);
		void execute(aeThreadState& threadInfo);

		/// Call a method on the given script object by its name
		void callMethod(aeon_object* object, const std::string& prototype);

		/// Call a method on the given script object by the method id
		void callMethod(aeon_object* object, uint32_t methodId);

		/// Call a script function
		void call(aeon_module& module, const char* func);	
};

#endif // aeon_vm_h__
