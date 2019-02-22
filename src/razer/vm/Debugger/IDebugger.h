#ifndef IDEBUGGER_H__
#define IDEBUGGER_H__

class RzThreadContext;

class IDebugger
{
public:
	virtual void onUncaughtExceptionThrown() { }
	virtual void onInterrupted() { }
	virtual void onBreakpoint()  { }

	RzThreadContext* ctx = nullptr;

	RzThreadContext* getCurrentContext()
	{
		return ctx;
	}

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
