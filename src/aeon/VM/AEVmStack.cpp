#include <AEON/VM/AEVmStack.h>
#include <AEON/VM/AEVm.h>

AEValue AEVmStack::popVar()
{
	AEValue* v = reinterpret_cast<AEValue*>(esp);
	esp += sizeof(AEValue);
	return *v;
}