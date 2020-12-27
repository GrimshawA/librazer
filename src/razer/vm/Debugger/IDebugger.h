#ifndef IDEBUGGER_H__
#define IDEBUGGER_H__

#include <razer/config.h>

class RzThreadContext;

class RZ_API IDebugger
{
public:
    IDebugger();
    virtual ~IDebugger();

	virtual void onUncaughtExceptionThrown() { }
	virtual void onInterrupted() { }
	virtual void onBreakpoint()  { }

	RzThreadContext* ctx = nullptr;

    RzThreadContext* getCurrentContext();

protected:
	// API for debugger impls
	void stepOver();
	void stepInto();
	void stepOut();

	void pause();
	void resume();

	int numThreads();
};

#endif
