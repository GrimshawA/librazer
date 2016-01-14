#ifndef aeon_compiler_h__
#define aeon_compiler_h__

#include "aeon_module.h"
#include "aeon_tree.h"
#include "aeon_expression.h"
#include <vector>
#include <stack>
#include <stdint.h>

class aeon_vm;
class aeon_context;

/**
	\class aeon_compiler
	\brief Generates byte code for aeon programs

	This compiler generates bytecode from validated AST
*/
class aeon_compiler
{
	public:

		// needed to retrieve function calling data
		aeon_vm* vm;
		aeon_context* env;

		aeon_module* mod;
		int currOffset = 0;

		struct FunctionLocalObjectInfo
		{
			std::string name;
		};

		struct BlockUnwindData
		{
			std::vector<int> localIndex;
		};

		/// Each block creates a scope with its own local vars, which has to be popped on the end of the block
		std::stack<BlockUnwindData> mBlockUnwindData;

		std::vector<FunctionLocalObjectInfo> mFunctionLocals;

		/// new

		struct LocalObjectInstance
		{
			int offset;
			int size;
			aeon_type* type;
			std::string name;
		};

		struct ScopeLocalData
		{
			std::vector<LocalObjectInstance> locals;
		};

		std::vector<ScopeLocalData> m_scopes;
		
	public:

		/// Emit a compiler class error of what happened
		void throwError(const std::string& message);

		int findLocalObject(const std::string& refname);

		/// Emit byte code for the passed AST
		void generate(aeon_ast_node* root);

		/// Get the cursor position, aka the index of the last added instruction
		int32_t cursor();

public:

		/// Emit an instruction
		void emit_instruction(aeon_instruction instr);

		/// Emits an instruction at the cursor from premade arguments
		uint32_t emit_instruction(uint8_t opcode, int8_t arg0 = 0, int8_t arg1 = 0, int8_t arg2 = 0);

		/// Emits the code to evaluate and prepare the expression
		void emitExpressionEval(aeon_expression* expression, int flag);

		/// Starts a new nested scope for locals
		void push_scope();

		/// Emits code for destructing the topmost scope level
		void pop_scope();

		/// Emit a local construction of a POD variable
		void emit_local_construct_pod(int32_t size);

		/// Emit a local construction of a OBJECT variable
		void emit_local_construct_object(int32_t size);

		/// Emits code to pop a scoped variable, considering all its type traits
		void emit_local_destruct(LocalObjectInstance& var);

		/// Emit a local destruction of an OBJECT variable
		void emit_local_destruct_object(int32_t size);

		/// Emit a local destruction of a POD variable
		void emit_local_destruct_pod(int32_t size);

		/// Emit code for a for loop execution
		void emitForLoop(ast_for* forloop);

		void compile_class(ast_class* clss);
		void compile_functioncall(ast_funccall* funccall);
		void compile_function(aeon_ast_function* func);
		void compile_block(ast_codeblock* codeblock);
		void compile_return(ast_return* ret);
		void compile_if(ast_ifbranch* cond);
		void emitWhileLoop(ast_while* whileloop);
		void compile_expreval(aeon_expression* expr);
		void compile_namespace(ast_namespace* namespace_node);
		void compile_global_var(ast_varexpr* global_var);
		void emitAssignOp(ast_binaryop* assign_expr);
		void emitPrefixIncrOp(ast_unaryop* expr);

};

#endif // aeon_compiler_h__
