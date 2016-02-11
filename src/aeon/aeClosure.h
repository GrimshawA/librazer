#ifndef aeClosure_h__
#define aeClosure_h__

#include "aeon_bytecode.h"
#include <aeon/aeVM.h>

#include <vector>

class aeClosure
{
public:

	aeVM*              m_vm;
	int                   m_executionId;
	int                   m_methodId;
	void*                 m_object;
	std::vector<vm_value> m_boundArgs;

public:

	/// Starts in invalid state and calls have no effect
	aeClosure();

	/// Detect if everything is alright with the closure to call
	bool check();

	/// Allows to check if the closure is in valid state currently
	operator bool();

	/// Call with the bound arguments
	void operator()();
};

#endif // aeClosure_h__
