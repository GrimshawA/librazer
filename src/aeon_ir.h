#ifndef aeon_ir_h__
#define aeon_ir_h__

#include <vector>
#include <stdint.h>

// Provisional interfaces for AEON IR for later usage

struct aeon_ir_instruction
{
	enum types
	{
		SSA_ASSIGN,
		SSA_COMPARE,
		STACK_OFFSET
	};

	union
	{
		struct assignment
		{
			int32_t var0;
			int32_t var1;
			int32_t var2;
		};

		struct stk_offset
		{
			int32_t off;
		};
	};

	int type;
};

struct aeon_ir_method
{
	std::vector<aeon_ir_instruction> instructions;
};

void run_all_optims(aeon_ir_method& method);

#endif // aeon_ir_h__
