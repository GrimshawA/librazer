#include <RzSDK/container/String.h>
#include <RazerVM/VirtualMachine.h>
#include <razer/bind_utils.h>
#include <Logger.h>

void registerString(RzModule* mod) {
    mod->registerType("String", sizeof(RzString));
    mod->registerTypeConstructor("String", [](void* memory, RzVirtualMachine* vm)
    {
        new (memory) RzString();
    });

    mod->registerMethod("String", "void set(int32)", [](RzGeneric g){
        RzString* obj = (RzString*)g.popObject();
        std::string str = g.unpack_string();
        obj->set(str);
        //RZLOG("STRING SETTED TO %s\n", str.c_str());
    });

    mod->registerMethod("String", "void print()", [](RzGeneric g){
        RzString* obj = (RzString*)g.popObject();
        obj->print();
    });

    mod->registerMethod("String", "void dbg(int32)", [](RzGeneric g){
        RzString* obj = (RzString*)g.popObject();
        auto val = g.popInt32();
        RZLOG("VALUE DBG: %d\n", val);
    });

    mod->registerMethod("String", "int size()", [](RzGeneric g){
        RzString* obj = (RzString*)g.popObject();
        obj->size();
    });

    mod->registerMethod("String", "String operatorAdd(String b)", [](RzGeneric g){
        RzString* a = (RzString*)g.popObject();
        RzString* b = (RzString*)g.popObject();
        RzString* result = new RzString();
        result->set(a->toStdString() + b->toStdString());
        g.pushObject(result);
    });

    mod->registerMethod2("String", "test", AutoMethodWrapper<decltype(&RzString::test), &RzString::test>::Invoke);
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

void RzString::test()
{
	RZLOG("Test worked!");
}

int RzString::size() {
    RZLOG("STRING SIZE %x\n", this);
    return m_str.size();
}
