#include "aeon_bytecode.h"

std::array<const char*, static_cast<int32_t>(EOpCodes::Count)> EOpCodeNames = {
	
};

void printBits(size_t const size, void const * const ptr)
{
	unsigned char *b = (unsigned char*)ptr;
	unsigned char byte;
	int i, j;

	for (i = size - 1; i >= 0; i--)
	{
		for (j = 7; j >= 0; j--)
		{
			byte = b[i] & (1 << j);
			byte >>= j;
			printf("%u", byte);
		}
	}
	puts("");
}

void setopcode(aeon_instruction& inst, EOpCodes opcode)
{
	inst.opcode = opcode;
	//printBits(sizeof(inst), &inst);
}

uint32_t getopcode(const aeon_instruction& inst)
{
	return inst.opcode;
}

uint32_t extract_uint32(const aeon_instruction& inst, uint64_t _lsb)
{
	const unsigned char* addr = reinterpret_cast<const unsigned char*>(&inst);
	uint64_t roi = *(int64_t*)(addr);
	uint64_t lsb = _lsb;
	uint64_t msb = lsb + 16;
	uint64_t result = (roi >> lsb) & ~(~0 << (msb - lsb + 1));
	return static_cast<uint32_t>(result);
}

void setinst_a(aeon_instruction& inst, int val_a)
{
	//inst = inst | (static_cast<uint64_t>(val_a) << 24);
	//printBits(sizeof(inst), &inst);
	inst.arg0 = val_a;
}

uint32_t getinst_a(const aeon_instruction& inst)
{
	return inst.arg0;
}

void setinst_b(aeon_instruction& inst, int val_b)
{
	inst.arg1 = val_b;
}

uint32_t getinst_b(const aeon_instruction& inst)
{
	return inst.arg1;
}
