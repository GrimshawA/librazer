#ifndef IDEBUGGER_H__
#define IDEBUGGER_H__

class IDebugger
{
public:
	virtual void onUncaughtExceptionThrown() { }
	virtual void onInterrupted() { }
	virtual void onBreakpoint()  { }

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
