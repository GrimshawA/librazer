#include <AEON/aeThreadState.h>
#include <AEON/Runtime/AEVm.h>

AEValue aeThreadState::popVar()
{
	AEValue* v = reinterpret_cast<AEValue*>(esp);
	esp += sizeof(AEValue);
	return *v;
}