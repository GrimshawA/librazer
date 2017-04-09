#include <RazerCore/container/String.h>
#include <RazerVM/VirtualMachine.h>
#include <Logger.h>

void registerString(RzModule* mod) {
    mod->registerType("String", sizeof(RzString));
    mod->registerTypeConstructor("String", [](void* memory, RzVirtualMachine* vm)
    {
        new (memory) RzString();
    });

    mod->registerMethod("String", "int size()", [](AEGeneric g){
        RzString* obj = (RzString*)g.unpack_ptr();
        obj->size();
    });
}

RzString::RzString() {
    RZLOG("STRING CONSTRUCTED\n\n");
}

int RzString::size() {
    RZLOG("STRING SIZE %x\n", this);
    return m_str.size();
}
