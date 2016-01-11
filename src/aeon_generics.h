#ifndef aeon_generics_h__
#define aeon_generics_h__

#include <vector>
#include <stdint.h>

class aeon_vm;

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

		int type;
};

	aeon_variant vmvalue_from_float(float v);
	aeon_variant vmvalue_from_stringindex(uint32_t poolIndex);
	void vmvalue_print(aeon_variant v);

	aeon_variant vmvalue_from_bool(bool b);

	struct atom_generic
	{
		aeon_vm* vm;

		int getarg_int(uint32_t index)
		{
			return static_cast<int>(args[index]._u64);
		}

		float getArgFloat(uint32_t index);

		void setarg_float(uint32_t index, float v)
		{
			args.resize(index + 1);
			args[index] = aeon_variant(v);
		}

		std::string getarg_string(uint32_t index);

		aeon_variant retarg;
		std::vector<aeon_variant> args;
		bool hasRet = false;
	};

#endif // aeon_generics_h__
