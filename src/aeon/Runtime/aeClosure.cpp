#include "aeClosure.h"

aeClosure::aeClosure()
: m_vm(nullptr)
{

}

void aeClosure::operator()()
{
	if (m_vm)
	{
		//m_vm->callMethod(m_object, m_methodId);
	}
}

bool aeClosure::check()
{
	return m_vm != nullptr;
}

aeClosure::operator bool()
{
	return check();
}