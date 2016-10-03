#ifndef aeon_compiler_h__
#define aeon_compiler_h__

#include <AEON/AST/Nodes.h>
#include <AEON/Compiler/aeCompilerConv.h>
#include <AEON/Runtime/AEModule.h>
#include <AEON/aeReportManager.h>

#include <vector>
#include <stack>
#include <stdint.h>

class AEVirtualMachine;
class AEContext;

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
	int mode = AE_VAR_INVALID; ///< Reference frame, or where the varable belongs to
	int offset = 0;            ///< Offset within reference frame
	aeQualType type;           ///< Qualified type of this object
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
	bool lvalue = false; ///< Must load as an editable value (loadaddress)
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
class AECompiler
{
public:
		AEContext*                       m_env;                   ///< The environment of modules for figuring interdependencies and other things
		AEModule*                        m_module;                ///< Current module being compiled
		AEType*                             m_currentStruct = nullptr;
		int                                 m_cursor = 0;            ///< Current index within the bytecode we are in
		std::vector<ScopeLocalData>         m_scopes;                ///< The stack of scopes to help compilation
		std::vector<AEStructNode*>           m_classes;               ///< Class we are compiling right now
		aeNodeFunction*                     m_caller;                ///< Current function node we're compiling
		AEFunction*                         m_currentFunction;       ///< Current function being compiled to
		int32_t                             m_OffsetFromBasePtr;     ///< How far are we from the base pointer
		TypeSystemInformation               m_typeSystem;            ///< Table that defines what can be converted to what and how
		aeReportManager                     m_reporter;
		bool                                m_logAllocs;
		bool                                m_logExprStmt;
		bool                                m_logExprOps;
		bool                                m_outputLogs;

	public:

		/// Construct initial values
		AECompiler();

		/// Emit a compiler class error of what happened
		void throwError(const std::string& errorCode, const std::string& message);

		int findLocalObject(const std::string& refname);

		/// Emit byte code for the passed AST
		void generate(AEBaseNode* root);

		/// Get the cursor position, aka the index of the last added instruction
		int32_t cursor();

public:

	bool canImplicitlyConvert(aeQualType origin, aeQualType dest);

	AEStructNode* getTopClassNode();

	/// Builds a qualified type identifier based on context
	aeQualType buildQualifiedType(const std::string& type);

	/// Find the qualified type of a given expression
	aeQualType buildQualifiedType(aeNodeExpr* e);
	
	/// Emit an instruction
	uint32_t emitInstruction(AEInstruction instr);

	/// Emits an instruction at the cursor from premade arguments
	uint32_t emitInstruction(uint8_t opcode, int8_t arg0 = 0, int8_t arg1 = 0, int8_t arg2 = 0);

	/// Starts a new nested scope for locals
	void push_scope();

	/// Emits code for destructing the topmost scope level
	void pop_scope();

	/// Declares a function local variable that can be addressed within it
	/// The stack frame of a function is basically a sequence of slots, each being a variable, at discrete offsets from the base pointer
	void declareStackVar(const std::string& name, aeQualType type);

	/// Pops the function arguments from the stack
	void releaseParametersContext();

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
	AEType* evaluateType(aeNodeExpr* expr);

	/// Evaluates the class node to an actual type
	AEType* evaluateType(AEStructNode* class_node);

	/// Evaluates a typename to a real type depending on context
	AEType* evaluateType(const std::string& type_name);

	/// Evaluate which function fn is trying to call (derived from context)
	AEFunction* selectFunction(aeNodeFunctionCall* fn);

	/// All the dirty tricks
	void emitDebugPrint(const std::string& message);

	/// Regarding scope, tries to deduce if we know how to convert typeB to typeA
	bool canConvertType(AEType* typeA, AEType* typeB);

	// High level constructs compilation
	void emitClassCode(AEStructNode* clss);
	AEFunction* compileFunction(aeNodeFunction* func);
	void emitNamespaceCode(aeNodeNamespace* namespace_node);
	void emitGlobalVarCode(aeNodeIdentifier* global_var);
	void emitStatement(AEStmtNode* stmt);
	void emitEnumValue(aeEnum* enumDef, const std::string& valueDef);
	void emitBreakpoint();
	void emitClassConstructors(AEType* classType, AEStructNode* classNode);
	void emitClassDestructors(AEType* classType, AEStructNode* classNode);
	void emitConstructorInjection(aeNodeFunction* node, AEFunction* function);


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
	void emitVarExpr(aeNodeIdentifier* var, const aeExprContext& parentExprContext);
	void emitLoadAddress(aeNodeExpr* expr);
	void emitLoadLiteral(aeNodeLiteral* lt);
	void emitMemberOp(aeNodeAccessOperator* acs);
	void emitImplicitConversion(aeQualType typeA, aeQualType typeB);
	void emitNew(aeNodeNew* newExpr);
	void emitSubscriptOp(aeNodeSubscript* subscript);
	void emitLambdaFunction(aeNodeFunction* function);
	void emitArithmeticOp(aeNodeBinaryOperator* op, const aeExprContext& context);
	void emitPushThis();
	void compileVarAssign(aeNodeExpr* lhs, aeNodeExpr* rhs);
	void loadVarRef(aeNodeExpr* e);

	/// Function calls
	void emitFunctionCall(aeQualType beingCalledOn, aeNodeFunctionCall* funccall, aeExprContext ctx);
	void emitVariantCall(aeNodeFunctionCall* fn);
	void emitLateBoundCall(aeNodeFunctionCall* fn);

};

#define CompilerLog(...) printf(__VA_ARGS__);
#define CompilerError(...) throwError(__VA_ARGS__);
#define CompilerWarning(...) printf(__VA_ARGS__);

#undef CompilerLog
#define CompilerLog

#endif // aeon_compiler_h__
