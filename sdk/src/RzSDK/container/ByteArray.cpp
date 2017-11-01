#include <RzSDK/container/ByteArray.h>
#include <RazerRuntime/RzEngine.h>
#include <RazerVM/VirtualMachine.h>

int32_t RzByteArray::size() {
    return m_data.size();
}

void RzByteArray::registerApi(RzModule& m) {
    m.registerType("ByteArray", sizeof(RzByteArray));
    m.registerTypeConstructor("ByteArray", [](void* memory, RzVirtualMachine* vm)
    {
        new (memory) RzByteArray;
    });

    m.registerMethod("ByteArray", "int32 size()", [](RzGeneric g){
        RzByteArray* obj = (RzByteArray*)g.popObject();
        int32_t s = obj->size();
        g.pushInt32(s);
    });
}
