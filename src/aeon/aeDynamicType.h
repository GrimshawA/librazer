#ifndef aeon_variant_h__
#define aeon_variant_h__

#include <string>
#include <vector>
#include <stdint.h>

/**
	\struct AEVmObject
	\brief This is the built-in variant type for the language.

	Each instance of this type carries its type and its value.
	Both can freely mutate between states.
	It can be an array, a prototype-based object
*/
struct AEVmObject
{
	enum
	{
		eInt8,
		eInt16,
		eInt32,
		eInt64,
		eU8,
		eU16,
		eU32,
		eU64,
		eFloat,
		eDouble,
		eObject
	};

	AEVmObject(float f)
	{
		_fp = f;
	}

	AEVmObject()
	{
		_i32 = 0;
	}

	union
	{

		int8_t _i8;
		int16_t _i16;
		int32_t _i32;
		int64_t _i64;
		uint8_t  _u8;
		uint16_t _u16;
		uint32_t _u32;
		uint64_t _u64;
		float    _fp;
		double   _dp;
		void*    _obj;
	};

	int8_t type; ///< 0 means null value
};

AEVmObject vmvalue_from_float(float v);
AEVmObject vmvalue_from_stringindex(uint32_t poolIndex);
void vmvalue_print(AEVmObject v);

AEVmObject vmvalue_from_bool(bool b);

#endif // aeon_variant_h__
