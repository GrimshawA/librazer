#ifndef AEVmThread_h__
#define AEVmThread_h__

#include <razer/vm/ThreadContext.h>
#include <thread>

/**
	\class RzThreadHandler
	\brief Manages one native thread for executing code

	These objects spawn new threads for executing instructions concurrently. 
	The full life cycle of the thread is managed here and each thread has its own
	context (and stack) to ensure safety.
*/
class RzThreadHandler
{
public:
	uint64_t stackSize();

	void start();

private:
	void threadFunc();

private:
	RzThreadContext m_context;
	std::thread m_thread;
};
#endif // AEVmThread_h__
