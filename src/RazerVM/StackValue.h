#ifndef RZSTACKVALUE_H__
#define RZSTACKVALUE_H__

#include <stdint.h>

/**
	When pushing operands to the stack they
	must be properly aligned and padded.
	This struct helps with this, making the code more
	portable and easier to handle.
*/
struct RzStackValue
{
	union
	{
		uint8_t  u8;
		int8_t   i8;
		uint16_t u16;
		int16_t  i16;
		uint32_t u32;
		int32_t  i32;
		uint64_t u64;
		int64_t  i64;
		float    fp;
		float    dp;
		void*    ptr;
	};

	static RzStackValue from_int32(int32_t v);
	static RzStackValue make_uint32(uint32_t v);
	static RzStackValue make_float(float v);
	static RzStackValue make_double(double v);
	static RzStackValue make_ptr(void* v);
};

#endif // RZSTACKVALUE_H__
