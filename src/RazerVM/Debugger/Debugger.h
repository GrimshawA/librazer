#ifndef DEBUGGER_H__
#define DEBUGGER_H__

#include <RazerVM/Debugger/IDebugger.h>

class Debugger : public IDebugger
{
public:
	void onUncaughtExceptionThrown() override;
	void onInterrupted() override;
	void onBreakpoint() override;
};

#endif
