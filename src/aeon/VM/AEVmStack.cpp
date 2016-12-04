#include <AEON/VM/AEVmStack.h>
#include <AEON/VM/AEVm.h>

void AEVmStack::pushVariant(const RzValue& v)
{
	printf("push variant at offset %d (%s)\n", ebp - esp, v.str().c_str());

	esp -= sizeof(v);
	new (esp) RzValue();
	*reinterpret_cast<RzValue*>(esp) = v;
}

void AEVmStack::popVariant(RzValue& v)
{
	v = *reinterpret_cast<RzValue*>(esp);
	reinterpret_cast<RzValue*>(esp)->~RzValue();
	esp += sizeof(RzValue);
}

void AEVmStack::pushVariantRef(const AEValueRef& ref)
{
	esp -= sizeof(ref);
	new (esp)AEValueRef();
	*reinterpret_cast<AEValueRef*>(esp) = ref;
}

AEValueRef AEVmStack::popVariantRef()
{
	AEValueRef v;
	v = *reinterpret_cast<AEValueRef*>(esp);
	reinterpret_cast<AEValueRef*>(esp)->~AEValueRef();
	esp += sizeof(AEValueRef);
	return v;
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
