#include <RazerCore/container/String.h>
#include <RazerVM/VirtualMachine.h>
#include <Logger.h>

void registerString(RzModule* mod) {
    mod->registerType("String", sizeof(RzString));
    mod->registerTypeConstructor("String", [](void* memory, RzVirtualMachine* vm)
    {
        new (memory) RzString();
    });

    mod->registerMethod("String", "void set(int32)", [](AEGeneric g){
        RzString* obj = (RzString*)g.unpack_ptr();
        std::string str = g.unpack_string();
        obj->set(str);
        //RZLOG("STRING SETTED TO %s\n", str.c_str());
    });

    mod->registerMethod("String", "void print()", [](AEGeneric g){
        RzString* obj = (RzString*)g.unpack_ptr();
        obj->print();
    });

    mod->registerMethod("String", "int size()", [](AEGeneric g){
        RzString* obj = (RzString*)g.unpack_ptr();
        obj->size();
    });
}

RzString::RzString() {
    RZLOG("STRING CONSTRUCTED\n\n");
}

void RzString::set(const std::string& content) {
    m_str = content;
}

std::string RzString::toStdString() {
    return m_str;
}

void RzString::print() {
    RZLOG("STRING PRINT %s\n", m_str.c_str());
}

int RzString::size() {
    RZLOG("STRING SIZE %x\n", this);
    return m_str.size();
}
