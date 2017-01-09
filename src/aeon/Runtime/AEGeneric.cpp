#include <AEON/Runtime/AEGeneric.h>
#include <RazerVM/VirtualMachine.h>
#include <AEON/Runtime/AEGeneric.h>

int32_t AEGeneric::unpack_int32()
{
	RzStackValue strIndex = m_threadCtx->pop_value();
	return strIndex.i32;
}

uint32_t AEGeneric::unpack_uint32()
{
	return 0;
}

float AEGeneric::unpack_float()
{
	return 0;
}

double AEGeneric::unpack_double()
{
	return 0;
}

std::string AEGeneric::unpack_string()
{
	RzStackValue strIndex = m_threadCtx->pop_value();
	std::string str = m_threadCtx->engine->string_literals[strIndex.i32];
	return str;
}

void* AEGeneric::unpack_ptr()
{
	RzStackValue stckValue = m_threadCtx->pop_value();
	return stckValue.ptr;
}

/*AEValue AEGeneric::unpackVariant()
{
	AEValue v;
	m_vm->m_stk.popVariant(v);
	return v;
}*/

void AEGeneric::pack_int32(int32_t value)
{
	RzStackValue val;
	val.i32 = value;
	m_threadCtx->push_value(val);
}

void AEGeneric::pack_uint32(uint32_t value)
{

}

void AEGeneric::pack_float(float value)
{

}

void AEGeneric::pack_double(double value)
{

}
void AEGeneric::pack_string(const std::string& value)
{
	if (m_variantCall)
	{
		RzValue str(value);
		m_threadCtx->pushVariant(str);
	}
	else
	{
		AEString* strValue = new AEString();

		RzStackValue strIndex;
		strIndex.ptr = strValue;
		m_threadCtx->push_value(strIndex);
	}
}