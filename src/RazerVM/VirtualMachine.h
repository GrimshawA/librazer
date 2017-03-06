#ifndef RZVIRTUALMACHINE_H__
#define RZVIRTUALMACHINE_H__

#include <RazerVM/ThreadHandler.h>
#include <Rzr/RzModule.h>
#include <RazerVM/ThreadContext.h>
#include <RazerRuntime/AEValueList.h>
#include <Rzr/RzEngine.h>
#include <RazerVM/Debugger/IDebugger.h>

#include <vector>
#include <stdint.h>

class AEObject;

#define STATE_ASSERT (assert((esp > m_stack.data() + m_stack.size()); assert((esp < m_stack.data())); 

/**
	\class RzVirtualMachine
	\brief The vm is in charge of executing the program in its byte code form

	The responsibility of the vm is to execute bytecode from the aeon language. With the exception of JIT
	compiled native code, once the host application gives control to an AEON function, the vm will be processing
	the byte code to do so, potentially calling into native code halfway.

	The vm also takes control of managing the multiple hardware threads launched from the language and employ all mechanisms 
	specified by the language and standard libraries.

	Different vm can be instanced but they need to be executing on different programs, its unsafe to use two different vm to call into
	the same modules. For virtually all applications, only one vm is needed and it will take care of everything.

	[Threads]
	The vm operates directly on the caller thread until vm inner threads are launched. When such happens,
	the new threads will be managed by the VM and run effectively new threads, managed by the vm automatically.

	The vm has one stack for itself, for the main host thread, and an additional independent stack per hardware thread launched.

	[Coroutines]
	Coroutines are implemented by preservation of call state for restoration.
*/
class RzVirtualMachine
{
public:
	// [API]

	RzVirtualMachine();
	RzVirtualMachine(RzEngine* context);

	void execute(int functionId);
	void executeAsync(int functionId);

public:

	RzValue call(RzValue obj, const std::string& functionName, AEValueList args = AEValueList());

	void setContext(RzEngine* context);


public:

	RzModule* get_current_mod();


	void prepare(aeFunctionId function);
	void pushThis(void* obj);

	/// Call a method on the given script object by its name
	void callMethod(AEObject* object, const std::string& prototype);

	/// Call a method on the given script object by the method id
	void callMethod(AEObject* object, uint32_t methodId);

	/// Call a script function
	void call(RzModule& module, const char* func);

	int call(AEFunction* fn);

	void startThread();

public:

	typedef std::shared_ptr<RzThreadHandler> RzThreadHandlerPtr;

	RzThreadContext                 m_mainContext;       ///< The context for the host thread (for host synchronous execution)
	std::vector<RzThreadHandlerPtr> m_threads;           ///< Hardware threads spawned by the vm
	RzEngine*                       m_ctx; ///< The engine that encapsulates all code the vm executes
	std::shared_ptr<IDebugger>      m_dbg;
};

#endif // RZVIRTUALMACHINE_H__
