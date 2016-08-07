#ifndef AEVmThread_h__
#define AEVmThread_h__

#include <AEON/VM/AEVmStack.h>
#include <thread>

class AEVmThread
{
public:
	uint64_t stackSize();

	void start();

private:
	void threadFunc();

private:
	AEVmStack stack;
	std::thread m_thread;
};

class AEVmThreadEnv
{
public:
	void* ptr;
	int func_id;
	int instigator_thread;
};

#endif // AEVmThread_h__
