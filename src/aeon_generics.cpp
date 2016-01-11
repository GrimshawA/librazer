#include "aeon_generics.h"
#include "aeon_vm.h"

aeon_variant vmvalue_from_float(float v)
{
	aeon_variant rv;
	rv.type = aeon_variant::eFloat;
	rv._fp = v;
	return rv;
}

	aeon_variant vmvalue_from_stringindex(uint32_t poolIndex)
	{
		aeon_variant rv;
		rv.type = aeon_variant::eU32;
		rv._u32 = poolIndex;
		return rv;
	}

	aeon_variant vmvalue_from_bool(bool b)
	{
		aeon_variant rv;
		rv.type = aeon_variant::eU32;
		rv._u32 = static_cast<uint32_t>(b);
		return rv;
	}

	void vmvalue_print(aeon_variant v)
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


	//////////////////////////////////////////////////////////////////////////

	float atom_generic::getArgFloat(uint32_t index)
	{
		return args[index]._fp;
	}

std::string atom_generic::getarg_string(uint32_t index)
{
	uint32_t strIndex = args[index]._u32;
	return vm->get_current_mod()->getStringFromPool(strIndex);
}