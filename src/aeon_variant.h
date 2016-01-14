#ifndef aeon_variant_h__
#define aeon_variant_h__

#include <string>
#include <vector>
#include <stdint.h>

/**
	\struct aeon_variant
	\brief Dynamically typed variable

	When dynamic typing is enabled in AEON, every value is a
	aeon_variant, which can be null or any of the types supported
	by the language.
*/
struct aeon_variant
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

	aeon_variant(float f)
	{
		_fp = f;
	}

	aeon_variant()
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

	uint32_t type;
};

aeon_variant vmvalue_from_float(float v);
aeon_variant vmvalue_from_stringindex(uint32_t poolIndex);
void vmvalue_print(aeon_variant v);

aeon_variant vmvalue_from_bool(bool b);

#endif // aeon_variant_h__
