#include "aeDynamicType.h"

AEVmObject vmvalue_from_float(float v)
{
	AEVmObject rv;
	rv.type = AEVmObject::eFloat;
	rv._fp = v;
	return rv;
}

AEVmObject vmvalue_from_stringindex(uint32_t poolIndex)
{
	AEVmObject rv;
	rv.type = AEVmObject::eU32;
	rv._u32 = poolIndex;
	return rv;
}

AEVmObject vmvalue_from_bool(bool b)
{
	AEVmObject rv;
	rv.type = AEVmObject::eU32;
	rv._u32 = static_cast<uint32_t>(b);
	return rv;
}

void vmvalue_print(AEVmObject v)
{
	/*switch (v.type)
	{
	case aeon_variant::eU8: Log("vmvalue(uint8): %d", v._u8); break;
	case aeon_variant::eU16: Log("vmvalue(uint16): %d", v._u16); break;
	case aeon_variant::eU32: Log("vmvalue(uint32): %d", v._u32); break;
	case aeon_variant::eU64: Log("vmvalue(uint64): %d", v._u64); break;
	case aeon_variant::eFloat: Log("vmvalue(float): %d", v._fp); break;
	case aeon_variant::eDouble: Log("vmvalue(double): %d", v._dp); break;
	}*/
}