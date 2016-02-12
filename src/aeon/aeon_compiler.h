#ifndef aeon_compiler_h__
#define aeon_compiler_h__

#include "aeCompilerConv.h"
#include "nodes/aeNodeFunction.h"
#include "nodes/aeNodeAccessOperator.h"
#include "nodes/aeNodeSubscript.h"
#include "nodes/aeNodeNew.h"
#include "nodes/aeNodeLiterals.h"
#include "nodes/aeNodeRef.h"
#include "nodes/aeNodeNew.h"
#include "nodes/aeNodeReturn.h"
#include "aeon_module.h"
#include "aeon_tree.h"
#include "aeReportManager.h"
#include <vector>
#include <stack>
#include <stdint.h>

class aeVM;
class aeon_context;

/**
	Any given variable can be a member of a class (field),
	a global or a function local that lives only during function execution.
*/
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
	aeQualType type;
	std::string name;
};

/*
	\class aeExprContext
	\brief Context for compiling expressions passed from node to node
*/
struct aeExprContext
{
	aeQualType expectedResult; /// The expression handler is responsible for leaving the right type and value on the stack

	aeNodeExpr* rootExpr;
	bool is_temporary = false; ///< If the expression is isolated, it should evaluate for side effects and leave nothing on the stack
	bool must_be_rvalue = false;
	bool rx_value = false; ///< Tells the expression it must load a temporary rvalue (to be consumed shortly after in expression handling)
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
class aeCompiler
{
public:
		aeon_context*                       m_env;                   ///< The environment of modules for figuring interdependencies and other things
		aeon_module*                        m_module;                ///< Current module being compiled
		int                                 m_cursor = 0;            ///< Current index within the bytecode we are in
		std::vector<ScopeLocalData>         m_scopes;                ///< The stack of scopes to help compilation
		std::vector<aeNodeClass*>           m_classes;               ///< Class we are compiling right now
		aeNodeFunction*                     m_caller;                ///< Current function node we're compiling
		aeFunction*                         m_currentFunction;       ///< Current function being compiled to
		int32_t                             m_OffsetFromBasePtr;     ///< How far are we from the base pointer
		TypeSystemInformation               m_typeSystem;            ///< Table that defines what can be converted to what and how
		aeReportManager                     m_reporter;
		bool                                m_logAllocs;
		bool                                m_logExprStmt;
		bool                                m_logExprOps;
		bool                                m_outputLogs;

	public:

		/// Construct initial values
		aeCompiler();

		/// Emit a compiler class error of what happened
		void throwError(const std::string& errorCode, const std::string& message);

		int findLocalObject(const std::string& refname);

		/// Emit byte code for the passed AST
		void generate(aeNodeBase* root);

		/// Get the cursor position, aka the index of the last added instruction
		int32_t cursor();

public:

	bool canImplicitlyConvert(aeQualType origin, aeQualType dest);

	aeNodeClass* getTopClassNode();

	/// Builds a qualified type identifier based on context
	aeQualType buildQualifiedType(const std::string& type);

	/// Find the qualified type of a given expression
	aeQualType buildQualifiedType(aeNodeExpr* e);
	
	/// Emit an instruction
	uint32_t emitInstruction(aeon_instruction instr);

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
	void destructLocalVar(VariableStorageInfo& var);

	/// Emit a local destruction of an OBJECT variable
	void emit_local_destruct_object(int32_t size);

	/// Get the variable info by its identifier name (respects the current compilation scope)
	VariableStorageInfo getVariable(std::string name);

	/// Evaluates the type of the expression/variable, taking into account the scope of the cursor
	/// Any given variable 'x' can have different types depending from where its referenced.
	/// Returns nullptr if it couldn't evaluate the scope
	aeType* evaluateType(aeNodeExpr* expr);

	/// Evaluates the class node to an actual type
	aeType* evaluateType(aeNodeClass* class_node);

	/// Evaluates a typename to a real type depending on context
	aeType* evaluateType(const std::string& type_name);

	/// All the dirty tricks
	void emitDebugPrint(const std::string& message);

	/// Regarding scope, tries to deduce if we know how to convert typeB to typeA
	bool canConvertType(aeType* typeA, aeType* typeB);

	// High level constructs compilation
	void emitClassCode(aeNodeClass* clss);
	void emitFunction(aeNodeFunction* func);
	void emitNamespaceCode(aeNodeNamespace* namespace_node);
	void emitGlobalVarCode(aeNodeRef* global_var);
	void emitStatement(aeNodeStatement* stmt);
	void emitBreakpoint();

	// Statement compilation
	void emitBlock(aeNodeBlock* codeblock);
	void emitReturnCode(aeNodeReturn* ret);
	void emitBranchCode(aeNodeBranch* cond);
	void emitWhileLoop(aeNodeWhile* whileloop);
	void emitForLoop(aeNodeFor* forloop);
	void emitVarDecl(const aeNodeVarDecl& varDecl);

	// Expression evaluation
	void emitExpressionEval(aeNodeExpr* expr, aeExprContext exprContext);
	void emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs);
	void emitPrefixIncrOp(aeNodeUnaryOperator* expr);
	void emitBinaryOp(aeNodeBinaryOperator* operation);
	void emitConditionalOp(aeNodeBinaryOperator* operation);
	void emitFunctionCall(aeQualType beingCalledOn, aeNodeFunctionCall* funccall, aeExprContext ctx);
	void emitVarExpr(aeNodeRef* var, const aeExprContext& parentExprContext);
	void emitLoadAddress(aeNodeExpr* expr);
	void emitLoadLiteral(aeNodeLiteral* lt);
	void emitMemberOp(aeNodeAccessOperator* acs);
	void emitImplicitConversion(aeQualType typeA, aeQualType typeB);
	void emitNew(aeNodeNew* newExpr);
	void emitSubscriptOp(aeNodeSubscript* subscript);
	void emitLambdaFunction(aeNodeFunction* function);
	void emitArithmeticOp(aeNodeBinaryOperator* op, const aeExprContext& context);
	void emitPushThis();
};

#define CompilerLog(...) printf(__VA_ARGS__);
#define CompilerError(...) throwError(__VA_ARGS__);
#define CompilerWarning(...) printf(__VA_ARGS__);

#undef CompilerLog
#define CompilerLog

#endif // aeon_compiler_h__
