#ifndef aeon_bytecode_h__
#define aeon_bytecode_h__

#include <vector>
#include <array>
#include <stdint.h>

enum EOpCodes
{
	OP_NEW,
	OP_CALL,		   ///< Call one aeon procedure, static, method, global, which assumes its input environment is already set
	OP_CALLVIRTUAL,	   ///< Call a virtual method on an aeon object, separate to avoid overhead of checking
	OP_CALLNATIVE,	   ///< Call a native function on the host environment
	OP_CALLMETHOD_NAT, ///< Call non-static member function on a native object
	OP_RETURN,           ///< Returns from a function immediately
	OP_JZ,            ///< Jump if eax == 0 to relative address (pc += offset)
	OP_JMP,        ///< jmp to an absolute location pc = x
	OP_PUSHARG,       ///< 
	OP_LOADK,         ///< load a constant into stack
	OP_MUL,           ///< mul stores in eax, ebx * ec
	OP_DIV,           ///< 
	OP_MOD,           ///<
	OP_ADD,           ///<
	OP_SUB,           ///<
 	OP_C_F_I32,           ///< Built in type conversion
	OP_C_I32_F,           ///< Built in type conversion
	OP_LT,            ///< compare less than, a < b
	OP_GT,            ///<
	OP_EQ,         ///< Test equality
	OP_NOT,        ///< Negate operand
	OP_SET,        ///< Assign something to something
	OP_LOAD,       ///< Pushes the contents of local variable <index> to stack (integer)
	OP_SIZEOF,     ///< Pushes the size in bytes to the stack of the given type token
	OP_NEWOBJECT,        ///< Creates a new object of the selected type and pushes its objectref
	OP_DELETEOBJECT,     ///< Release the objectref on the stack
	OP_MOV,
	OP_DTEST,      ///< Just debug prints something for tests
	OP_DEBUG,       ///< Dirty tricks for debug mode
	Count
};

enum AeonConstants
{
	AEK_ESP,
	AEK_EBP,
	AEK_THIS,
};

enum AeonOperatorConstants
{
	AE_PODCOPY,
	AE_CUSTOMCOPY,
	AE_CUSTOMOPERATOR
};

enum AeonExpressionEvaluationRules
{
	AE_LVALUE,   ///< The expression must evaluate to an address that can be edited
	AE_NOSTORE,  ///< The expression can evaluate but leave the stack with no results
	AE_RVALUE,   ///< The expression MUST leave a result in stack
};

enum AeonAddressingModes
{
	AE_GLOBALS, ///< To load global objects
	AE_LOCALS,  ///< To load objects within function scope
	AE_OBJECT,  ///< To load objects within an object's scope
};

enum AeonLiteralTypes
{
	AEK_STRING,
	AEK_INT,
	AEK_FLOAT
};

/// Atom VM pointer type. Whenever stack operations that need arbitrary objects happen
/// they use an objectref object.
typedef void* atom_objectref;

extern std::array<const char*, static_cast<int32_t>(EOpCodes::Count)> EOpCodeNames;

const char* getopname(int opcode);

struct aeon_object_ref
{
	void* addr;
};

struct aeon_instruction
{
	uint8_t opcode;
	int8_t arg0;
	int8_t arg1;
	int8_t arg2;
};

void setopcode(aeon_instruction& inst, EOpCodes opcode);
uint32_t getopcode(const aeon_instruction& inst);

void setinst_a(aeon_instruction& inst, int val_a);
uint32_t getinst_a(const aeon_instruction& inst);

void setinst_b(aeon_instruction& inst, int val_b);
uint32_t getinst_b(const aeon_instruction& inst);

#endif // aeon_bytecode_h__
