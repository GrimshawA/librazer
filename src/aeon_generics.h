#ifndef aeon_generics_h__
#define aeon_generics_h__

#include <vector>
#include <stdint.h>

#include "aeDynamicType.h"

class aeon_vm;

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
			args[index] = aeDynamicType(v);
		}

		std::string getarg_string(uint32_t index);

		aeDynamicType retarg;
		std::vector<aeDynamicType> args;
		bool hasRet = false;
	};

#endif // aeon_generics_h__
