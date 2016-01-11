#ifndef aeon_compiler_h__
#define aeon_compiler_h__

#include "aeon_module.h"
#include "aeon_tree.h"
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
			int type;
			std::string name;
		};

		struct ScopeConstructionData
		{
			std::vector<LocalObjectInstance> locals;
		};

		std::vector<ScopeConstructionData> scopes;
		
	public:

		int findLocalObject(const std::string& refname);

		/// Emit byte code for the passed AST
		void generate(atom_ast_node* root);

		/// Emit an instruction
		void emit_instruction(aeon_instruction instr);

		/// Starts a new nested scope for locals
		void emit_scope_begin();

		/// Emits code for destructing the topmost scope level
		void emit_scope_end();

		/// Emit a local construction of a POD variable
		void emit_local_construct_pod(int32_t size);

		/// Emit a local destruction of a POD variable
		void emit_local_destruct_pod(int32_t size);

		/// Emit a local construction of a OBJECT variable
		void emit_local_construct_object(int32_t size);

		/// Emit a local destruction of an OBJECT variable
		void emit_local_destruct_object(int32_t size);

		void compile_class(ast_class* clss);
		void compile_functioncall(ast_funccall* funccall);
		void compile_function(ast_func* func);
		void compile_block(ast_codeblock* codeblock);
		void compile_blockend();
		void compile_return(ast_return* ret);
		void compile_if(ast_ifbranch* cond);
		void compile_while(ast_while* whileloop);
		void compile_expreval(ast_expr* expr);
		void compile_namespace(ast_namespace* namespace_node);
		void compile_global_var(ast_varexpr* global_var);
		void compile_assignment(ast_binaryop* assign_expr);

};

#endif // aeon_compiler_h__
