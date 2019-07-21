#include <razer/vm/ThreadContext.h>
#include <razer/vm/VirtualMachine.h>
#include <razer/utils/Logger.h>
#include <cstring>

RzModule& RzThreadContext::getModule() {
	return *cl->module;
}

void RzThreadContext::pushVariant(const RzValue& v) {
	RZLOG("push variant at offset %d (%s)\n", ebp - esp, v.str().c_str());

	esp -= sizeof(v);
	new (esp) RzValue();
	*reinterpret_cast<RzValue*>(esp) = v;
}

void RzThreadContext::popVariant(RzValue& v) {
	v = *reinterpret_cast<RzValue*>(esp);
	reinterpret_cast<RzValue*>(esp)->~RzValue();
	esp += sizeof(RzValue);
}

void RzThreadContext::pushVariantRef(const AEValueRef& ref) {
	esp -= sizeof(ref);
	new (esp)AEValueRef();
	*reinterpret_cast<AEValueRef*>(esp) = ref;
}

AEValueRef RzThreadContext::popVariantRef() {
	AEValueRef v;
	v = *reinterpret_cast<AEValueRef*>(esp);
	reinterpret_cast<AEValueRef*>(esp)->~AEValueRef();
	esp += sizeof(AEValueRef);
	return v;
}

void RzThreadContext::pushMemory(const std::vector<uint8_t>& mem) {
	esp -= mem.size();
	memcpy(esp, mem.data(), mem.size());
}

std::vector<uint8_t> RzThreadContext::popMemory(int size) {
	std::vector<uint8_t> mem(size);
	memcpy(mem.data(), esp, size);
	esp += size;
	return mem;
}

void RzThreadContext::pushObject(AEObject* obj) {
    RZLOG("pushed %d bytes : object %x\n", sizeof(obj->m_obj), obj->m_obj);
	esp -= sizeof(obj->m_obj);
	memcpy(esp, &obj->m_obj, sizeof(obj->m_obj));
}

void RzThreadContext::alloc(int size) {
	esp -= size;
}
