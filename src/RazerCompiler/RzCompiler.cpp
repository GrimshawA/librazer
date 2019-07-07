#include <RazerCompiler/RzCompiler.h>
#include <RazerCompiler/TypeResolver.h>
#include <razer/vm/InstructionSet.h>
#include <razer/vm/VirtualMachine.h>
#include <RazerRuntime/RzEngine.h>
#include <DebugDefs.h>
#include <Logger.h>

#include <cassert>

void debugCodeRange(RzModule* module, int start, int end)
{
    RZLOG("Code %d to %d\n", start, end);
    for (int i = start; i < end; ++i)
    {
        auto& in = module->m_code[i];
        RZLOG("%s %d %d %d\n", inst_opcode_str(in).c_str(), in.arg0, in.arg1, in.arg2);
    }
}

RzCompiler::RzCompiler()
    : m_currentFunction(nullptr)
    , m_outputLogs(false)
{

    m_logAllocs = true;
    m_logExprStmt = true;
    m_logExprOps = true;

    m_OffsetFromBasePtr = 0;
    m_caller = nullptr;
}

int32_t RzCompiler::cursor()
{ 
    return m_cursor;
}

bool RzCompiler::canImplicitlyConvert(RzQualType origin, RzQualType dest)
{
    for (auto& conv : m_typeSystem.m_table)
    {
        if (conv.allow_implicit && conv.T1 == origin.getType() && conv.T2 == dest.getType())
            return true;
    }
    return false;
}

void RzCompiler::emitEnumValue(aeEnum* enumDef, const std::string& valueDef)
{
    if (enumDef->table.find(valueDef) == enumDef->table.end())
    {
        //		CompilerError("0010", "The value '" + valueDef + "' is not a part of the enum '" + enumDef->getName() + "'");
        return;
    }
    emitInstruction(OP_LOADENUM, enumDef->table[valueDef]);
}

RzQualType RzCompiler::buildQualifiedType(const std::string& type)
{
    RzQualType qtype;
    qtype.m_type = m_env->getTypeInfo(type);
    return qtype;
}

RzQualType RzCompiler::buildQualifiedType(aeNodeExpr* e)
{
    return resolveQualifiedType(*this, *e, RzQualType());
}

uint32_t RzCompiler::emitInstruction(RzInstruction instr)
{
    m_module->m_code.push_back(instr);
    return m_cursor++;
}

uint32_t RzCompiler::emitInstruction(uint8_t opcode, int8_t arg0, int8_t arg1, int8_t arg2)
{
    RzInstruction instr;
    instr.opcode = opcode;
    instr.arg0 = arg0;
    instr.arg1 = arg1;
    instr.arg2 = arg2;
    m_module->m_code.push_back(instr);
    return m_cursor++;
}

AEStructNode* RzCompiler::getTopClassNode()
{
    if (m_classes.size() > 0)
        return m_classes[m_classes.size() - 1];
    return nullptr;
}

void RzCompiler::push_scope()
{
    ScopeLocalData newScope;
    m_scopes.push_back(newScope);
}

void RzCompiler::pop_scope()
{
    assert(m_scopes.size() != 0);

    ScopeLocalData& top = m_scopes.back();
    for (auto it = top.locals.rbegin(); it != top.locals.rend(); ++it)
    {
        destructLocalVar(*it);
    }
    m_scopes.pop_back();
}

void RzCompiler::declareStackVar(const std::string& name, RzQualType type)
{
    int bpOffset = m_OffsetFromBasePtr;

    VariableStorageInfo localObject;
    localObject.type = type;
    localObject.name = name;
    localObject.offset = bpOffset;
    localObject.mode = AE_VAR_LOCAL;
    m_scopes.back().locals.push_back(localObject);

    m_OffsetFromBasePtr += type.getSize();

#if defined TRACE_STACK
    RZLOG("STACK VAR: %s offset %d\n", name.c_str(), bpOffset);
#endif
}

void RzCompiler::releaseParametersContext()
{
    //pop_scope();

    emitInstruction(OP_MOV, AEK_ESP, sizeof(RzStackValue) * m_currentFunction->params.size());
}

VariableStorageInfo RzCompiler::getVariable(std::string name)
{
    // Let's see if this is a local variable
    for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it)
    {
        // Is the variable we reference here?
        for (auto var_it = (*it).locals.rbegin(); var_it != (*it).locals.rend(); ++var_it)
        {
            if ((*var_it).name == name)
            {
                return (*var_it);
            }
        }
    }

    // Let's see if this is a local variable
    if (m_classes.size() > 0)
    {
        AEStructNode* classNode = getTopClassNode();
        RzType* type = evaluateType(classNode);

        auto field = type->getField(name);

        RzQualType actualType;

        for (auto& f : classNode->m_fields)
        {
            if (f->name == name)
            {
                actualType = resolveQualifiedType(*this, *f->declaration);
            }
        }

        if (field)
        {
            // The variable we are referencing is a member of 'this'
            VariableStorageInfo varStorage;
            varStorage.type = actualType;
            varStorage.mode = AE_VAR_FIELD;
            varStorage.offset = field->offset;
            varStorage.name = name;
            return varStorage;
        }
    }

    return VariableStorageInfo();
}

void RzCompiler::destructLocalVar(VariableStorageInfo& var)
{
    if (var.type.getType()->is_native)
    {
        //emitDebugPrint("Destroying a " + var.name);
        //emitInstrugction(OP_LOAD, AEK_ESP);
        //emitInstruction(OP_CALLMETHOD_NAT, m_env->getNativeBehaviorIndex(var.type.getName(), "~f"));
        emitInstruction(OP_MOV, AEK_ESP, var.type.getSize());
    }
    else
    {
        if (var.type.str() == "var")
        {
            emitInstruction(OP_POPVAR);
        }
        else
        {
            emitInstruction(OP_MOV, AEK_ESP, var.type.getSize());
        }
    }
}

void RzCompiler::emitBreakpoint()
{
    emitInstruction(OP_BREAKPOINT);
}

void RzCompiler::emit_local_construct_pod(int32_t size)
{

}

void RzCompiler::emit_local_construct_object(int32_t size)
{

}

bool RzCompiler::generate(RzSourceUnit* root)
{
    m_typeSystem.init(this);

    // Entry point, let's set a proper offset
    m_cursor = m_module->m_code.size();

    RzCompileResult ret;

    for (std::size_t i = 0; i < root->m_items.size(); ++i)
    {
        if (root->m_items[i]->m_nodeType == AEN_FUNCTION)
        {
            compileFunction(static_cast<aeNodeFunction*>(root->m_items[i]));

        }
        else if (root->m_items[i]->m_nodeType == AEN_CLASS)
        {
            ret = compileStruct(static_cast<AEStructNode*>(root->m_items[i]));
            if (ret == RzCompileResult::aborted)
                break;
        }
        else if (root->m_items[i]->m_nodeType == AEN_NAMESPACE)
        {
            emitNamespaceCode(static_cast<aeNodeNamespace*>(root->m_items[i]));
        }
        else if (root->m_items[i]->m_nodeType == AEN_IDENTIFIER)
        {
            emitGlobalVarCode(static_cast<aeNodeIdentifier*>(root->m_items[i]));
        }
        else if (root->m_items[i]->m_nodeType == AEN_IMPORT)
        {
            ret = compileImport((aeNodeImport&)*root->m_items[i]);
            if (ret == RzCompileResult::aborted)
                break;
        }

        if (ret.m_status == RzCompileResult::ABORTED)
            break;
    }

    for (std::size_t i = 0; i < root->m_types.size(); ++i) {
        ret = compileStruct(static_cast<AEStructNode*>(root->m_types[i].get()));
        if (ret == RzCompileResult::aborted)
            break;
    }

    if (ret.m_status == RzCompileResult::ABORTED)
    {
        RZLOG("Compilation finished with errors.\n");
        return false;
    }
    else
    {
        RZLOG("Compilation finished\n");
        return true;
    }

    return true;
}

void RzCompiler::emitNamespaceCode(aeNodeNamespace* namespace_node)
{
    for (std::size_t i = 0; i < namespace_node->m_items.size(); ++i)
    {
        if (namespace_node->m_items[i]->m_nodeType == AEN_FUNCTION)
        {
            compileFunction(static_cast<aeNodeFunction*>(namespace_node->m_items[i]));
        }
        else if (namespace_node->m_items[i]->m_nodeType == AEN_CLASS)
        {
            compileStruct(static_cast<AEStructNode*>(namespace_node->m_items[i]));
        }
        else if (namespace_node->m_items[i]->m_nodeType == AEN_NAMESPACE)
        {
            emitNamespaceCode(static_cast<aeNodeNamespace*>(namespace_node->m_items[i]));
        }
        else if (namespace_node->m_items[i]->m_nodeType == AEN_IDENTIFIER)
        {
            emitGlobalVarCode(static_cast<aeNodeIdentifier*>(namespace_node->m_items[i]));
        }
    }
}

void RzCompiler::emitGlobalVarCode(aeNodeIdentifier* global_var)
{

}

RzCompileResult RzCompiler::compileStruct(AEStructNode* clss)
{
    m_classes.push_back(clss);

    if (clss->parents.size() > 1 && !m_env->m_config.allowMultipleInheritance)
    {
        CompilerError("0002", "The class '" + clss->m_name + "' can't inherit from multiple classes.");
        return RzCompileResult::aborted;
    }

    RzType* typeInfo = m_module->getType(clss->m_name);
    typeInfo->m_absoluteName = clss->m_name;
    m_env->typedb.push_back(typeInfo);
    m_module->m_types.push_back(typeInfo);
    RzEngine::object_heap objectHeap;
    objectHeap.type = typeInfo;
    m_env->object_heaps.push_back(objectHeap);

    m_currentStruct = typeInfo;

    clss->m_typeInfo = typeInfo;

    CompilerLog("Compiled type %s\n", typeInfo->m_name.c_str());

    for (auto pnt : clss->parents)
    {
        RzType* info = m_env->getTypeInfo(pnt.parentClass);
        if (!info)
        {
            CompilerLog("Compiler Error C0001: Can't inherit from '%s'. No such type.\n", pnt.parentClass.c_str());
        }
    }

    int fldOffset = 0;
    for (std::size_t i = 0; i < clss->m_fields.size(); ++i) {
        auto fieldType = resolveQualifiedType(*this, *clss->m_fields[i]->declaration);

        aeField fld;
        fld.name = clss->m_fields[i]->name;
        fld.type = clss->m_fields[i]->type;
        fld.offset = fldOffset;

        assert(fieldType.getSize() > 0);

        fldOffset += fieldType.getSize();

        typeInfo->m_fields.push_back(fld);
    }

    typeInfo->computeMetrics();

    bool compiledConstructors = false;
    for (auto& func : clss->m_functions)
    {
        auto* funcNode = static_cast<aeNodeFunction*>(func.get());
        compiledConstructors |= funcNode->is_constructor;
    }
    // Now the class is compiled, we generate constructor and destructor
    if (!compiledConstructors)
    {
        emitClassConstructors(typeInfo, clss);
    }

    for (std::size_t i = 0; i < clss->m_functions.size(); ++i)
    {
         auto* funcNode = static_cast<aeNodeFunction*>(clss->m_functions[i].get());
        RzFunction* fn = compileFunction(funcNode);
        if (!fn) {
            RZLOG("Failed to compile function\n");
            return RzCompileResult::aborted;
        }
    }

    emitClassDestructors(typeInfo, clss);

    m_classes.pop_back();

    m_currentStruct = nullptr;

    return RzCompileResult::ok;
}

void RzCompiler::emitClassConstructors(RzType* classType, AEStructNode* classNode)
{
	RzFunction f;
	f.m_offset = m_cursor;
	f.m_module = m_module;
	f.m_absoluteName = classType->getName() + "." + classType->getName();

	emitDebugPrint("Constructor is being run!!!");
	emitReturnCode(nullptr);

	m_module->m_functions.emplace_back(f);

    RzType::MethodInfo method;
    method.offset = m_cursor;
    method.name = f.m_absoluteName;
    method.native = false;
    classType->m_methods.push_back(method);

	RZLOG("Emitted default constructor: %s index taken %d\n", f.m_absoluteName.c_str(), m_module->m_functions.size() - 1);

    aeNodeFunction* funcNode = new aeNodeFunction();
    funcNode->m_name = classType->getName();
    funcNode->is_constructor = true;
    classNode->m_functions.emplace_back(funcNode);

    /*aeNodeFunction* defaultConstructor = classNode->getMethod(classNode->m_name);

    if (!classType->isPod())
    {
        // Need to inject construction code into the constructors
        if (!defaultConstructor)
        {
            defaultConstructor = classNode->createDefaultConstructor();
        }

    //	injectMemberConstruction(defaultConstructor, classType, classNode);
    }*/
}

void RzCompiler::emitClassDestructors(RzType* classType, AEStructNode* classNode)
{

}

void RzCompiler::throwError(const std::string& errorCode, const std::string& message)
{
    //m_reporter.emitLog("error C" + errorCode + ": " + message);

    if (m_currentFunction)
    {
        m_currentFunction->m_compiled = false; // Compilation failed
    }
}

RzCompileResult RzCompiler::emitBranchCode(aeNodeBranch* cond)
{
    aeNodeExpr* test_expr = cond->m_expression.get();
    aeNodeBlock* nested_code = cond->m_block.get();

    RzQualType t = resolveQualifiedType(*this, *test_expr);

    // The expression must evaluate first
    emitExpressionEval(test_expr, RzExprContext::temporaryRValue());

    if (t.getName() != "bool") {
        RzCompileResult r = implicitConvert(t, m_env->getTypeInfo("bool"));
        if (r == RzCompileResult::aborted)
            return r;
    }

    // Now that the expression is evaluated and stored in a register, let's set the jmp
    emitInstruction(OP_JZ, 0, 0);
    int jmptestpc = m_cursor - 1;

    // Now let's define the actual block
    RzCompileResult r = emitBlock(nested_code);
    if (r == RzCompileResult::aborted)
        return r;

    // make sure the if jmp goes to the end of the block if it fails
    setinst_a(m_module->m_code[jmptestpc], (m_cursor - 1) - jmptestpc);

    return RzCompileResult::ok;
}

int RzCompiler::findLocalObject(const std::string& refname) {
    /*for (std::size_t i = 0; i < mFunctionLocals.size(); ++i)
    {
        if (mFunctionLocals[i].name == refname)
            return i;
    }*/
    return -1;
}

void RzCompiler::emitPrefixIncrOp(aeNodeUnaryOperator* expr)
{

}

RzCompileResult RzCompiler::emitForLoop(aeNodeFor* forloop)
{
    emitDebugPrint("FOR STARTED");

    push_scope();

    // Emit the code that initializes the control vars
    // They are automatically scoped and are destructed in the end of the loop
    emitStatement(forloop->initStatement.get());

    int pc_expreval = cursor(); // track the first instruction of this while, to jump back to evaluation each iteration

    RzExprContext ectx;
    ectx.must_be_rvalue = true;
    auto e = forloop->expr.get();
    emitExpressionEval(e, ectx);

    // Evaluate the expression
    int jzInstrIndex = emitInstruction(OP_JZ, 0, 0, 0);

    emitDebugPrint("FOR CYCLE START");
    emitBlock(forloop->block.get());
    emitDebugPrint("FOR CYCLE END");

    // After the code executes, apply the update expression
    emitExpressionEval(forloop->incrExpr.get(), ectx);

    // After the block terminates normally, we always go back up to expression evaluation
    // when the expression fails, it will jump right away to after this
    int jumpOffset = cursor() - pc_expreval + 1;
    jumpOffset = -jumpOffset;
    emitInstruction(OP_JMP, jumpOffset);

    // if the expression evaluates false, jump to after the for
    auto& jzInstr = m_module->m_code[jzInstrIndex];
    assert(jzInstr.opcode == OP_JZ);
    setinst_a(jzInstr, cursor() - jzInstrIndex - 1);

    pop_scope();
    emitDebugPrint("FOR ENDED");
    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::emitWhileLoop(aeNodeWhile* whileloop) {
    int pc_expreval = cursor();

    RzExprContext ectx;
    ectx.must_be_rvalue = true;
    emitExpressionEval(whileloop->expr.get(), ectx);

    int jmptestpc = emitInstruction(OP_JZ, 0, 0, 0);

    RzCompileResult r = emitBlock(whileloop->block.get());
    if (r == RzCompileResult::aborted)
        return r;

    // After the block terminates normally, we always go back up to expression evaluation
    // when the expression fails, it will jump right away to after this
    int jumpOffset = cursor() - pc_expreval + 1;
    jumpOffset = -jumpOffset;
    emitInstruction(OP_JMP, jumpOffset);

    // if the expression evaluates false, jump to after the while OP_JZ
    setinst_a(m_module->m_code[jmptestpc], (cursor() - 1) - jmptestpc);

    debugCodeRange(m_module, pc_expreval, cursor());

    return RzCompileResult::ok;
}

void RzCompiler::emitConstructorInjection(aeNodeFunction* node, RzFunction* function)
{
	emitDebugPrint("Constructor injected section begin");
    // This is where the initialization is done, before any user constructor instruction is executed
    AEStructNode* cl = getTopClassNode();

    for (auto& field : cl->m_fields)
    {
        emitDebugPrint("INIT " + field->name);
        RZLOG("INIT %s\n", field->name.c_str());

        if (!field->declaration)
            continue;

        auto fieldType = resolveQualifiedType(*this, *field->declaration);

        if (!fieldType.isPrimitive())
        {
            aeNodeExpr* lhs = new aeNodeIdentifier(field->name);
            aeNodeExpr* rhs = new aeNodeNew(field->declaration);
            node->m_block->prepend(new aeNodeBinaryOperator(lhs, rhs, "="));
        }

        /*if (!field.type.isPod())
        {
            emitPushThis();
            emitInstruction(OP_LOADADDR, AEK_THIS, field.offset);

            if (field.type.isTemplated())
            {
                // Templated types get the type as parameter
                for (int i = 0; i < field.type.getNumTemplateArgs(); ++i)
                {
                    RzQualType targ = field.type.getTemplateArg(i);
                    emitInstruction(OP_TYPEINFO, m_env->getTypeInfoIndex(targ.m_type));
                }
            }

            aeNodeFunctionCall fnCall;
            fnCall.m_name = "array";
            //emitFunctionCall(field.type, &fnCall, aeExprContext());

            RZLOG("INJECTED CONSTRUCTION %s\n", field.name.c_str());
        }*/
    }

	emitDebugPrint("Constructor injected section end");
	emitPushThis();
}

RzCompileResult RzCompiler::compileImport(aeNodeImport& node)
{
    RzModule* importedModule = m_env->resolveModule(node.symbol);

    if (!importedModule)
    {
        RZLOG("Couldn't resolve the import '%s' to any module\n", node.symbol.c_str());
        return RzCompileResult(RzCompileResult::ABORTED);
    }

    RZLOG("Import resolved to %s\n", importedModule->getName().c_str());

    m_module->createDependency(importedModule);

    return RzCompileResult();
}

void RzCompiler::emitLambdaFunction(aeNodeFunction* function)
{
    // todo
}

RzCompileResult RzCompiler::emitBlock(aeNodeBlock* codeblock)
{
    emitStackCanaryBegin();
    push_scope();

    for (std::size_t i = 0; i < codeblock->m_items.size(); ++i)
    {
        // emitDebugTrace();
        AEStmtNode& stmt = *static_cast<AEStmtNode*>(codeblock->m_items[i]);
        auto ret = emitStatement(&stmt);
        if (ret == RzCompileResult::aborted) {
            pop_scope();
            return ret;
        }

        emitDebugTrace();
    }

    pop_scope();
    emitStackCanaryEnd();
}

RzCompileResult RzCompiler::emitStatement(AEStmtNode* stmt) {
    /*
        List of statements accepted as "standalone" within blocks of code
        - switch
        - for
        - while
        - if
        - assignment
        - function calls (f(x) or (...).f(X)
        - declaration statement
    */

    switch (stmt->m_nodeType) {

    case AEN_FUNCTIONCALL: {
        aeNodeExpr a;
        return emitFunctionCall(a, RzQualType(), static_cast<aeNodeFunctionCall*>(stmt), RzExprContext());
    }

    case AEN_BRANCH:
        return emitBranchCode(static_cast<aeNodeBranch*>(stmt));

    case AEN_WHILE:
        return emitWhileLoop(static_cast<aeNodeWhile*>(stmt));

    case AEN_FOR:
        return emitForLoop(static_cast<aeNodeFor*>(stmt));

    case AEN_VARDECL:
        return compileVarDecl(*static_cast<aeNodeVarDecl*>(stmt));

    case AEN_BINARYOP: {
        if (((aeNodeBinaryOperator*)stmt)->oper == "=") {
            //RZLOG("error: Cannot assign\n");
            return emitAssignOp(((aeNodeBinaryOperator*)stmt)->operandA, ((aeNodeBinaryOperator*)stmt)->operandB);
        }
        else {
            RZLOG("The operation is not allowed as a top level statement\n");
            return RzCompileResult::aborted;
        }
    }
    case AEN_ACCESSOPERATOR: {
        if (((aeNodeAccessOperator*)stmt)->m_b->m_nodeType == AEN_FUNCTIONCALL) {

            return emitMemberOp(((aeNodeAccessOperator*)stmt));
        } else {
            RZLOG("The operation is not allowed as a top level statement\n");
            return RzCompileResult::aborted;
        }
    }
    default: {
        RZLOG("error: NOT compiling other expression '%s'\n", ((aeNodeExpr*)stmt)->str().c_str());
        return RzCompileResult::aborted;
    }
    }
}

RzCompileResult RzCompiler::emitReturnCode(aeNodeReturn* ret) {
    emitInstruction(OP_RETURN);
    return RzCompileResult::ok;
}

RzType* RzCompiler::evaluateType(aeNodeExpr* expr) {
    return nullptr;
}

RzType* RzCompiler::evaluateType(const std::string& type_name)
{
    // Walk the scope stack from current to top level to search the decl
    for (auto it = m_scopes.rbegin(); it != m_scopes.rend(); ++it)
    {
        // Is the variable we reference here?
        for (auto var_it = (*it).locals.rbegin(); var_it != (*it).locals.rend(); ++var_it)
        {
            if ((*var_it).name == type_name)
            {
                return (*var_it).type.getType();
            }
        }
    }

    // Are we compiling a method currently?
    // In that case we probably want a member variable
    if (!m_classes.empty())
    {
        AEStructNode* container_class = m_classes[m_classes.size() - 1];
        RzType* class_type = evaluateType(container_class);

        // todo
    }

    /// Can still be a primitive
    RzType* type = m_env->getTypeInfo(type_name);

    /// Cannot evaluate.
    return type;
}

RzType* RzCompiler::evaluateType(AEStructNode* class_node)
{
    return class_node->m_typeInfo;
}

void RzCompiler::emitStackCanaryBegin() {
    emitInstruction(OP_DEBUG, DBG_STACKCANARYBEGIN);
}

void RzCompiler::emitStackCanaryEnd() {
    emitInstruction(OP_DEBUG, DBG_STACKCANARYEND);
}

void RzCompiler::emitDebugPrint(const std::string& message) {
    emitInstruction(OP_DEBUG, DBG_LOG, m_env->getStringLiteral(message));
}

void RzCompiler::emitDebugTrace() {
    emitInstruction(OP_DEBUG, DBG_TRACE);
}
