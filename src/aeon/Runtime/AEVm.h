#ifndef aeon_vm_h__
#define aeon_vm_h__

#include <AEON/Runtime/AEModule.h>
#include <AEON/aeThreadState.h>
#include <AEON/Runtime/AEContext.h>

#include <vector>
#include <stdint.h>


class aeVM;
class AEObject;

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

		AEContext*           m_ctx;
		aeThreadState           m_stk;

	public:

		aeVM();

		aeVM(AEContext* context);

		AEModule* get_current_mod();

		void setContext(AEContext* context);

		void prepare(aeFunctionId function);
		void pushThis(void* obj);
		void execute(aeThreadState& threadInfo);

		/// Call a method on the given script object by its name
		void callMethod(AEObject* object, const std::string& prototype);

		/// Call a method on the given script object by the method id
		void callMethod(AEObject* object, uint32_t methodId);

		/// Call a script function
		void call(AEModule& module, const char* func);

		void call(AEFunction* fn);
};

#endif // aeon_vm_h__
