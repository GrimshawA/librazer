#ifndef aeon_bytecode_h__
#define aeon_bytecode_h__

#include <vector>
#include <array>
#include <stdint.h>

enum EOpCodes
{
	OP_NEW,
	OP_CALL,		   ///< Call a global or static class function
	OP_CALLNATIVE,	       ///< Call a native function on the host environment
	OP_CALLMETHOD,   ///< Call non-static member function on an object
	OP_CALLMETHOD_NAT, ///< Call non-static member function on a native object
	callvirt,	   ///< Call a virtual method on an object
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
 	f2i,           ///<
	i2f,           ///<
	OP_LT,            ///< compare less than, a < b
	OP_GT,            ///<
	OP_SET,        ///< Assign something to something
	OP_LOAD,       ///< Pushes the contents of local variable <index> to stack (integer)
	ilocal,        ///< Pushing a new scoped local variable (integer)
	push_obj,      ///< Pushes enough memory to stack to fit the object type
	pop_obj,       ///< Pop the memory of the given object type on top
	iloadaddr,     ///< Pushes an integer local address to stack
	poplocal,      ///< Pops stack to release local vars
	OP_SIZEOF,     ///< Pushes the size in bytes to the stack of the given type token
	invsign,       ///< Pops a int32 from stack and pushes back its inverse (n * -1)
	push_thisobj,  ///< Push an objectref for the contextual this ptr
	OP_NEWOBJECT,        ///< Creates a new object of the selected type and pushes its objectref
	OP_DELETEOBJECT,     ///< Release the objectref on the stack
	isinst,        ///< Checks if an object is an instance of class X
	OP_MOV,
	OP_DTEST,      ///< Just debug prints something for tests
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
