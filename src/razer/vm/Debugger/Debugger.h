#ifndef DEBUGGER_H__
#define DEBUGGER_H__

#include <razer/vm/Debugger/IDebugger.h>
#include <string>


class Debugger : public IDebugger
{
public:
	void onUncaughtExceptionThrown() override;
	void onInterrupted() override;
	void onBreakpoint() override;

public:

    void run();

	void handleCommand(const std::string& command);
};

#endif
