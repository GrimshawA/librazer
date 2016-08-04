#ifndef aeon_vm_h__
#define aeon_vm_h__

#include <AEON/Runtime/AEModule.h>
#include <AEON/aeThreadState.h>
#include <AEON/AEContext.h>

#include <vector>
#include <stdint.h>


class aeVM;
class AEObject;

#define STATE_ASSERT (assert((esp > m_stack.data() + m_stack.size()); assert((esp < m_stack.data())); 

/**
	\class aeon_vm
	\brief The vm is in charge of executing the program in its byte code form

	The responsibility of the vm is to execute bytecode from the aeon language. With the exception of JIT
	compiled native code, once the host application gives control to an AEON function, the vm will be processing
	the byte code to do so, potentially calling into native code halfway.

	The vm also takes control of managing the multiple hardware threads launched from the language and employ all mechanisms 
	specified by the language and standard libraries.

	Different vm can be instanced but they need to be executing on different programs, its unsafe to use two different vm to call into
	the same modules. For virtually all applications, only one vm is needed and it will take care of everything.
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
