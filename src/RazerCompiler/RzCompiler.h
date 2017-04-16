#ifndef RZCOMPILER_H__
#define RZCOMPILER_H__

#include <RazerCompiler/CompileResult.h>
#include <RazerParser/AST/Nodes.h>
#include <RazerCompiler/aeCompilerConv.h>
#include <Rzr/RzModule.h>
#include <RazerCompiler/BuildReport.h>

#include <vector>
#include <stack>
#include <stdint.h>

class RzVirtualMachine;
class RzEngine;

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
    \class RzCompiler
    \brief Compiles source code into bytecode
*/
class RzCompiler
{
public:
    RzEngine*                       m_env;                   ///< The environment of modules for figuring interdependencies and other things
    RzModule*                        m_module;                ///< Current module being compiled
    RzType*                             m_currentStruct = nullptr;
    int                                 m_cursor = 0;            ///< Current index within the bytecode we are in
    std::vector<ScopeLocalData>         m_scopes;                ///< The stack of scopes to help compilation
    std::vector<AEStructNode*>           m_classes;               ///< Class we are compiling right now
    aeNodeFunction*                     m_caller;                ///< Current function node we're compiling
    RzFunction*                         m_currentFunction;       ///< Current function being compiled to
    int32_t                             m_OffsetFromBasePtr;     ///< How far are we from the base pointer
    TypeSystemInformation               m_typeSystem;            ///< Table that defines what can be converted to what and how
    RzBuildReport                     m_reporter;
    bool                                m_logAllocs;
    bool                                m_logExprStmt;
    bool                                m_logExprOps;
    bool                                m_outputLogs;

public:

    /// Construct initial values
    RzCompiler();

    /// Emit a compiler class error of what happened
    void throwError(const std::string& errorCode, const std::string& message);

    int findLocalObject(const std::string& refname);

    /// Emit byte code for the passed AST
    bool generate(AEBaseNode* root);

    /// Get the cursor position, aka the index of the last added instruction
    int32_t cursor();

public:

    void collect(RzSourceUnit& parseTree);
    void collect(AEStructNode& cls);

    bool canImplicitlyConvert(aeQualType origin, aeQualType dest);

    AEStructNode* getTopClassNode();

    /// Builds a qualified type identifier based on context
    aeQualType buildQualifiedType(const std::string& type);

    /// Find the qualified type of a given expression
    aeQualType buildQualifiedType(aeNodeExpr* e);

    /// Emit an instruction
    uint32_t emitInstruction(RzInstruction instr);

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
    RzType* evaluateType(aeNodeExpr* expr);

    /// Evaluates the class node to an actual type
    RzType* evaluateType(AEStructNode* class_node);

    /// Evaluates a typename to a real type depending on context
    RzType* evaluateType(const std::string& type_name);

    /// Evaluate which function fn is trying to call (derived from context)
    RzFunction* selectFunction(aeNodeFunctionCall* fn);

    /// Regarding scope, tries to deduce if we know how to convert typeB to typeA
    bool canConvertType(RzType* typeA, RzType* typeB);

    // High level constructs compilation
    RzCompileResult compileStruct(AEStructNode* clss);
    RzFunction* compileFunction(aeNodeFunction* func);
    void emitNamespaceCode(aeNodeNamespace* namespace_node);
    void emitGlobalVarCode(aeNodeIdentifier* global_var);
    RzCompileResult emitStatement(AEStmtNode* stmt);
    void emitEnumValue(aeEnum* enumDef, const std::string& valueDef);
    void emitBreakpoint();
    void emitClassConstructors(RzType* classType, AEStructNode* classNode);
    void emitClassDestructors(RzType* classType, AEStructNode* classNode);
    void emitConstructorInjection(aeNodeFunction* node, RzFunction* function);
    RzCompileResult compileImport(aeNodeImport& node);


    // Statement compilation
    RzCompileResult emitBlock(aeNodeBlock* codeblock);
    RzCompileResult emitReturnCode(aeNodeReturn* ret);
    RzCompileResult emitBranchCode(aeNodeBranch* cond);
    RzCompileResult emitWhileLoop(aeNodeWhile* whileloop);
    RzCompileResult emitForLoop(aeNodeFor* forloop);
    RzCompileResult compileVarDecl(const aeNodeVarDecl& varDecl);

    // Expression evaluation
    RzCompileResult emitExpressionEval(aeNodeExpr* expr, aeExprContext exprContext);
    RzCompileResult emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs);
    void emitPrefixIncrOp(aeNodeUnaryOperator* expr);
    void emitBinaryOp(aeNodeBinaryOperator* operation);
    void emitConditionalOp(aeNodeBinaryOperator* operation);
    void emitVarExpr(aeNodeIdentifier* var, const aeExprContext& parentExprContext);
    void emitLoadAddress(aeNodeExpr* expr);
    void emitLoadLiteral(aeNodeLiteral* lt);
    RzCompileResult emitMemberOp(aeNodeAccessOperator* acs);
    void emitImplicitConversion(aeQualType typeA, aeQualType typeB);
    RzCompileResult compileNew(aeNodeNew& newExpr);
    void emitSubscriptOp(aeNodeSubscript* subscript);
    void emitLambdaFunction(aeNodeFunction* function);
    void emitArithmeticOp(aeNodeBinaryOperator* op, const aeExprContext& context);
    void emitPushThis();
    RzCompileResult compileVarAssign(aeNodeExpr* lhs, aeNodeExpr* rhs);
    void loadVarRef(aeNodeExpr* e);
    RzCompileResult compileVariantStackAlloc(const std::string& identifier, aeNodeExpr* initExpr);
    RzCompileResult compileStaticAssign(aeNodeExpr& lhs, aeNodeExpr& rhs);

    /// Function calls
    RzCompileResult emitFunctionCall(aeNodeExpr& selfExpr, aeQualType beingCalledOn, aeNodeFunctionCall* funccall, aeExprContext ctx);
    void compileVariantCall(aeNodeExpr* lhs, aeNodeFunctionCall* fn);
    void emitLateBoundCall(aeNodeFunctionCall* fn);
    RzCompileResult compileStaticObjectCall(aeNodeExpr& selfExpr, aeQualType obj, aeNodeFunctionCall& call);
    RzCompileResult compileNativeObjectCall(int moduleIndex, RzType::MethodInfo info);
    RzCompileResult compileArgsPush(const std::vector<aeNodeExpr*> args);

    /// Push the implicit this as a call argument
    RzCompileResult pushImplicitThis(aeNodeExpr& expr);

    /// DEBUG HELPERS
    /// Stack canaries will ensure the stack looks the same after an operation
    void emitStackCanaryBegin();
    void emitStackCanaryEnd();
    void emitDebugPrint(const std::string& message);
    void emitDebugTrace();


    RzBuildReport* m_report;

};

#define CompilerLog(...) printf(__VA_ARGS__);
#define CompilerError(...) throwError(__VA_ARGS__);
#define CompilerWarning(...) printf(__VA_ARGS__);

#define RZASSERTCOMPILER(r) if (r.status == RzCompileResult::aborted) return r;

#undef CompilerLog
#define CompilerLog

#endif // RZCOMPILER_H__
