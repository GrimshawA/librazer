#include <RazerRuntime/Generic.h>
#include <RazerVM/VirtualMachine.h>

int32_t RzGeneric::popInt32() {
    RzStackValue strIndex = m_threadCtx->pop_value();
    return strIndex.i32;
}

bool RzGeneric::popBoolean() {
    RzStackValue strIndex = m_threadCtx->pop_value();
    return (bool)strIndex.i32;
}

uint32_t RzGeneric::unpack_uint32() {
    return 0;
}

float RzGeneric::popFloat() {
    RzStackValue stckValue = m_threadCtx->pop_value();
    return stckValue.fp;
}

double RzGeneric::unpack_double() {
    return 0;
}

std::string RzGeneric::unpack_string() {
    RzStackValue strIndex = m_threadCtx->pop_value();
    std::string str = m_threadCtx->engine->string_literals[strIndex.i32];
    return str;
}

void* RzGeneric::popObject() {
    RzStackValue stckValue = m_threadCtx->pop_value();
    return stckValue.ptr;
}

/*AEValue AEGeneric::unpackVariant()
{
    AEValue v;
    m_vm->m_stk.popVariant(v);
    return v;
}*/

void RzGeneric::pushInt32(int32_t value) {
    RzStackValue val;
    val.i32 = value;
    m_threadCtx->push_value(val);
}

void RzGeneric::pushBoolean(bool value) {
    RzStackValue val;
    val.i32 = (int32_t)value;
    m_threadCtx->push_value(val);
}

void RzGeneric::pack_uint32(uint32_t value) {

}

void RzGeneric::pushFloat(float value) {
    RzStackValue val;
    val.fp = value;
    m_threadCtx->push_value(val);
}

void RzGeneric::pack_double(double value) {

}

void RzGeneric::pack_string(const std::string& value) {
    if (m_variantCall) {
        RzValue str(value);
        m_threadCtx->pushVariant(str);
    }
    else {
        AEString* strValue = new AEString();

        RzStackValue strIndex;
        strIndex.ptr = strValue;
        m_threadCtx->push_value(strIndex);
    }
}

void RzGeneric::pushObject(void* obj) {
    RzStackValue strIndex;
    strIndex.ptr = obj;
    m_threadCtx->push_value(strIndex);
}
