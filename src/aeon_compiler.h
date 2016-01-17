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

enum EVariableStorageLocations
{
	AE_VAR_INVALID,
	AE_VAR_LOCAL,
	AE_VAR_FIELD,
	AE_VAR_GLOBAL
};

struct VariableStorageInfo
{
	int mode = AE_VAR_INVALID;
	int offset = 0;
	int offset_bp = 0;
	aeon_type* type;
	std::string name;
};

struct TypeConversionStrategy
{
	enum ConversionModes
	{
		Primitive,
		CustomOverload,
		NativeCustomOverload,
	};

	bool allow_implicit;
	bool two_ways;
	aeon_type* typeA;
	aeon_type* typeB;
	int conversion_mode;
};

struct BinaryOperatorRelationship
{
	int op;
	aeon_type* lhs;
	aeon_type* rhs;
	aeon_type* dest_type;
	bool commutative;
	int priority;     ///< to keep it sorted by precedence, so we can ensure certain overloads are used instead of others
};

struct ExpressionEvalContext
{
	aeon_expression* rootExpr;
	bool is_temporary = false; ///< If the expression is isolated, it should evaluate for side effects and leave nothing on the stack
	bool must_be_rvalue = false;
};

struct ScopeLocalData
{
	std::vector<VariableStorageInfo> locals;
	int offset = 0;
};

/**
	\class aeon_compiler
	\brief Generates byte code for aeon programs

	This compiler generates bytecode from validated AST
*/
class aeon_compiler
{
public:
		aeon_context*                       m_env;                   ///< The environment of modules for figuring interdependencies and other things
		aeon_module*                        m_module;                ///< Current module being compiled
		int                                 m_cursor = 0;            ///< Current index within the bytecode we are in
		std::vector<ScopeLocalData>         m_scopes;                ///< The stack of scopes to help compilation
		std::vector<ast_class*>             m_classes;               ///< Class we are compiling right now
		int32_t                             m_OffsetFromBasePtr;     ///< How far are we from the base pointer
		std::vector<TypeConversionStrategy> m_typeConversionTable;   ///< Table that defines what can be converted to what and how

	public:

		/// Construct initial values
		aeon_compiler();

		/// Emit a compiler class error of what happened
		void throwError(const std::string& message);

		int findLocalObject(const std::string& refname);

		/// Emit byte code for the passed AST
		void generate(aeon_ast_node* root);

		/// Get the cursor position, aka the index of the last added instruction
		int32_t cursor();

public:

		/// Emit an instruction
		void emitInstruction(aeon_instruction instr);

		/// Emits an instruction at the cursor from premade arguments
		uint32_t emitInstruction(uint8_t opcode, int8_t arg0 = 0, int8_t arg1 = 0, int8_t arg2 = 0);

		/// Starts a new nested scope for locals
		void push_scope();

		/// Emits code for destructing the topmost scope level
		void pop_scope();

		/// Emit a local construction of a POD variable
		void emit_local_construct_pod(int32_t size);

		/// Emit a local construction of a OBJECT variable
		void emit_local_construct_object(int32_t size);

		/// Emits code to pop a scoped variable, considering all its type traits
		void emit_local_destruct(VariableStorageInfo& var);

		/// Emit a local destruction of an OBJECT variable
		void emit_local_destruct_object(int32_t size);

		/// Emit a local destruction of a POD variable
		void emit_local_destruct_pod(int32_t size);

		/// Get the variable info by its identifier name (respects the current compilation scope)
		VariableStorageInfo getVariable(std::string name);

		/// Evaluates the type of the expression/variable, taking into account the scope of the cursor
		/// Any given variable 'x' can have different types depending from where its referenced.
		/// Returns nullptr if it couldn't evaluate the scope
		aeon_type* evaluateType(aeon_expression* expr);

		/// Evaluates the class node to an actual type
		aeon_type* evaluateType(ast_class* class_node);

		/// Evaluates a typename to a real type depending on context
		aeon_type* evaluateType(const std::string& type_name);

		/// Regarding scope, tries to deduce if we know how to convert typeB to typeA
		bool canConvertType(aeon_type* typeA, aeon_type* typeB);

		// High level constructs compilation
		void emitClassCode(ast_class* clss);
		void emitFunctionCode(aeon_ast_function* func);
		void emitNamespaceCode(ast_namespace* namespace_node);
		void emitGlobalVarCode(ast_varexpr* global_var);

		// Statement compilation
		void emitScopeCode(ast_codeblock* codeblock);
		void emitReturnCode(ast_return* ret);
		void emitBranchCode(ast_ifbranch* cond);
		void emitWhileLoop(ast_while* whileloop);
		void emitForLoop(ast_for* forloop);
		void emitVarDecl(aeon_stmt_vardecl* varDecl);

		// Expression evaluation
		void emitExpressionEval(aeon_expression* expr, ExpressionEvalContext exprContext);
		void emitAssignOp(aeon_expression* lhs, aeon_expression* rhs);
		void emitPrefixIncrOp(ast_unaryop* expr);
		void emitBinaryOp(ast_binaryop* operation);
		void emitConditionalOp(ast_binaryop* operation);
		void emitFunctionCall(ast_funccall* funccall, ExpressionEvalContext ctx);
		void emitVarExpr(ast_varexpr* var);
		void emitLoadAddress(aeon_expression* expr);
		void emitLoadLiteral(ast_literal* lt);
		void emitConversion(aeon_type* typeA, aeon_type* typeB);
};

#endif // aeon_compiler_h__
