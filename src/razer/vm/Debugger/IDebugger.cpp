#include <razer/vm/Debugger/IDebugger.h>

IDebugger::IDebugger()
{

}

IDebugger::~IDebugger()
{

}

void IDebugger::stepOver()
{

}

void IDebugger::stepInto()
{

}

void IDebugger::stepOut()
{

}

void IDebugger::pause()
{

}

void IDebugger::resume()
{

}

int IDebugger::numThreads()
{
	return 0;
}

RzThreadContext* IDebugger::getCurrentContext()
{
    return ctx;
}
