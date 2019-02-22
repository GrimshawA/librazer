#include <razer/vm/Debugger/Debugger.h>
#include <razer/vm/ThreadContext.h>

#include <string>
#include <iostream>

void Debugger::onUncaughtExceptionThrown()
{

}

void Debugger::onInterrupted()
{

}

void Debugger::onBreakpoint()
{
	std::cout << "Halting on breakpoint" << std::endl;

	while(true)
	{
		std::cout << ">>> ";
		std::string cmd;
		std::cin >> cmd;

		if (cmd == "continue")
		{
			return;
		}

		handleCommand(cmd);
	}
}

void Debugger::handleCommand(const std::string& command)
{
	if (command == "trace")
	{
		auto* ctx = getCurrentContext();
		for (auto& frame : ctx->frames)
		{
			std::cout << "Frame: " << frame.getName() << std::endl;
		}
		return;
	}

	std::cout << "Unknown command: " << command << std::endl;
}