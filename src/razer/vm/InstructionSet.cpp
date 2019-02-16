#include <razer/vm/InstructionSet.h>
#include <Logger.h>
#include <string>

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
			RZLOG("%u", byte);
		}
	}
	puts("");
}

void setopcode(RzInstruction& inst, EOpCodes opcode)
{
	inst.opcode = opcode;
	//printBits(sizeof(inst), &inst);
}

uint32_t getopcode(const RzInstruction& inst)
{
	return inst.opcode;
}

uint32_t extract_uint32(const RzInstruction& inst, uint64_t _lsb)
{
	const unsigned char* addr = reinterpret_cast<const unsigned char*>(&inst);
	uint64_t roi = *(int64_t*)(addr);
	uint64_t lsb = _lsb;
	uint64_t msb = lsb + 16;
	uint64_t result = (roi >> lsb) & ~(~0 << (msb - lsb + 1));
	return static_cast<uint32_t>(result);
}

void setinst_a(RzInstruction& inst, int val_a)
{
	//inst = inst | (static_cast<uint64_t>(val_a) << 24);
	//printBits(sizeof(inst), &inst);
	inst.arg0 = val_a;
}

uint32_t getinst_a(const RzInstruction& inst)
{
	return inst.arg0;
}

void setinst_b(RzInstruction& inst, int val_b)
{
	inst.arg1 = val_b;
}

uint32_t getinst_b(const RzInstruction& inst)
{
	return inst.arg1;
}

std::string inst_opcode_str(RzInstruction inst)
{
	switch (inst.opcode)
	{
	case OP_NEW: return "OP_NEW";
	case OP_CALL: return "OP_CALL";
	case OP_JMP: return "OP_JMP";
	case OP_JZ: return "OP_JZ";
    case OP_SUB: return "OP_SUB";
    case OP_AND: return "OP_AND";
    case OP_OR: return "OP_OR";
    case OP_LOAD: return "OP_LOAD";
    case OP_LOADADDR: return "OP_LOADADDR";
    case OP_CONV: return "OP_CONV";
    case OP_SET: return "OP_SET";
	case OP_CALLNATIVE: return "OP_CALLNATIVE";
	case OP_NATIVECALL: return "OP_CALLMETHOD_NAT";
	case OP_RETURN: return "OP_RETURN";
	case OP_DEBUG: return "OP_DEBUG";
	case OP_VARCALL: return "OP_VARCALL";
	case OP_PUSHVAR: return "OP_PUSHVAR";
	case OP_VARLOAD: return "OP_VARLOAD";
	case OP_VARLOADREF: return "OP_VARLOADREF";
	case OP_VARSTORE: return "OP_VARSTORE";
	case OP_POPVAR: return "OP_POPVAR";
	case OP_LOADK: return "OP_LOADK";
	}
	return std::to_string(inst.opcode);
}

std::string getPrettyInstructionStr(RzInstruction inst) {
	std::string s;

	s += inst_opcode_str(inst);

	if (inst.opcode == OP_LOAD)
	{
		if (inst.arg1 == AEP_INT32) {
			s += "INT32";
		}
	}

	if (inst.opcode == OP_SET)
	{
		if (inst.arg2 == AEP_INT32) {
			s += " INT32";
		}
	}

	if (inst.opcode == OP_LOADK)
	{
		if (inst.arg0 == AEK_INT)
		{
			s += " INT32 ";
		}
	}

	return s;
}