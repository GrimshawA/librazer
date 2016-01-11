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
	call_method_n, ///< Call non-static member function on a native object
	callvirt,	   ///< Call a virtual method on an object
	ret,           ///< Returns from a function immediately
	jz,            ///< Jump if eax == 0 to relative address (pc += offset)
	jmp,           ///< jmp to an absolute location pc = x
	PushArg,       ///< 
	loadk,         ///< load a constant into stack
	mul,           ///< mul stores in eax, ebx * ec
	OP_DIV,           ///< 
	mod,           ///<
	add,           ///<
	sub,           ///<
 	f2i,           ///<
	i2f,           ///<
	lt,            ///< compare less than, a < b
	gt,            ///<
	set,           ///< Assign something to something
	iload,         ///< Pushes the contents of local variable <index> to stack (integer)
	ilocal,        ///< Pushing a new scoped local variable (integer)
	push_obj,      ///< Pushes enough memory to stack to fit the object type
	pop_obj,       ///< Pop the memory of the given object type on top
	iloadaddr,     ///< Pushes an integer local address to stack
	poplocal,      ///< Pops stack to release local vars
	objsizeof,     ///< Pushes the size in bytes to the stack of the given type token
	invsign,       ///< Pops a int32 from stack and pushes back its inverse (n * -1)
	push_thisobj,  ///< Push an objectref for the contextual this ptr
	newobj,        ///< Creates a new object of the selected type and pushes its objectref
	deleteobj,     ///< Release the objectref on the stack
	isinst,        ///< Checks if an object is an instance of class X
	Count
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
	uint8_t arg0;
	uint8_t arg1;
	uint8_t arg2;
};

void setopcode(aeon_instruction& inst, EOpCodes opcode);
uint32_t getopcode(const aeon_instruction& inst);

void setinst_a(aeon_instruction& inst, int val_a);
uint32_t getinst_a(const aeon_instruction& inst);

void setinst_b(aeon_instruction& inst, int val_b);
uint32_t getinst_b(const aeon_instruction& inst);

aeon_instruction create_instruction(int opcode);

#endif // aeon_bytecode_h__
