#include <AEON/VM/AEVmStack.h>
#include <AEON/VM/AEVm.h>

void AEVmStack::pushVariant(const AEValue& v)
{
	esp -= sizeof(v);
	new (esp) AEValue();
	*reinterpret_cast<AEValue*>(esp) = v;
}

void AEVmStack::popVariant(AEValue& v)
{
	v = *reinterpret_cast<AEValue*>(esp);
	reinterpret_cast<AEValue*>(esp)->~AEValue();
	esp += sizeof(AEValue);
}
