#include <AEON/VM/AEVmStack.h>
#include <AEON/VM/AEVm.h>

void AEVmStack::pushVariant(const AEValue& v)
{
	printf("push variant at offset %d (%s)\n", ebp - esp, v.str().c_str());

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

void AEVmStack::pushMemory(const std::vector<uint8_t>& mem)
{
	esp -= mem.size();
	memcpy(esp, mem.data(), mem.size());
}

std::vector<uint8_t> AEVmStack::popMemory(int size)
{
	std::vector<uint8_t> mem(size);
	memcpy(mem.data(), esp, size);
	esp += size;
	return mem;
}

void AEVmStack::pushObject(AEObject* obj)
{
	printf("pushed %d bytes : object\n", sizeof(obj->m_obj));
	esp -= sizeof(obj->m_obj);
	memcpy(esp, &obj->m_obj, sizeof(obj->m_obj));
}

void AEVmStack::alloc(int size)
{
	esp -= size;
}
