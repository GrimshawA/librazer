#include <AEON/Runtime/AEGeneric.h>
#include <RazerVM/VirtualMachine.h>
#include <AEON/Runtime/AEGeneric.h>

int32_t AEGeneric::unpack_int32()
{
	RzStackValue strIndex = m_vm->m_mainContext.pop_value();
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
	RzStackValue strIndex = m_vm->m_mainContext.pop_value();
	std::string str = m_vm->m_ctx->string_literals[strIndex.i32];
	return str;
}

/*AEValue AEGeneric::unpackVariant()
{
	AEValue v;
	m_vm->m_stk.popVariant(v);
	return v;
}*/

void AEGeneric::pack_int32(int32_t value)
{

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
	
}