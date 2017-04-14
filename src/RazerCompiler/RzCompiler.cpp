#include <RazerCompiler/RzCompiler.h>
#include <RazerVM/InstructionSet.h>
#include <RazerVM/VirtualMachine.h>
#include <Rzr/RzEngine.h>
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

bool RzCompiler::canImplicitlyConvert(aeQualType origin, aeQualType dest)
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

aeQualType RzCompiler::buildQualifiedType(const std::string& type)
{
    aeQualType qtype;
    qtype.m_type = m_env->getTypeInfo(type);
    return qtype;
}

aeQualType RzCompiler::buildQualifiedType(aeNodeExpr* e)
{
    return e->getQualifiedType(this);
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

void RzCompiler::declareStackVar(const std::string& name, aeQualType type)
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
    pop_scope();
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

        if (type->getField(name))
        {
            // The variable we are referencing is a member of 'this'
            VariableStorageInfo varStorage;
            varStorage.type = type->getField(name)->type;
            varStorage.mode = AE_VAR_FIELD;
            varStorage.offset = type->getField(name)->offset;
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

bool RzCompiler::generate(AEBaseNode* root)
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
            ret = emitClassCode(static_cast<AEStructNode*>(root->m_items[i]));
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
            emitClassCode(static_cast<AEStructNode*>(namespace_node->m_items[i]));
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

RzCompileResult RzCompiler::emitClassCode(AEStructNode* clss)
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

    for (std::size_t i = 0; i < clss->m_fields.size(); ++i) {
        aeField fld;
        fld.name = clss->m_fields[i]->name;
        fld.type = clss->m_fields[i]->type;
        typeInfo->m_fields.push_back(fld);
    }

    typeInfo->computeMetrics();

    for (std::size_t i = 0; i < clss->m_functions.size(); ++i)
    {
        AEFunction* fn = compileFunction(static_cast<aeNodeFunction*>(clss->m_functions[i].get()));
        if (!fn)
            return RzCompileResult::aborted;
    }

    for (std::size_t i = 0; i < clss->m_items.size(); ++i)
    {
        if (clss->m_items[i]->m_nodeType == AEN_CLASS)
        {
            emitClassCode(static_cast<AEStructNode*>(clss->m_items[i]));
        }
        else if (clss->m_items[i]->m_nodeType == AEN_VARDECL)
        {
            aeNodeVarDecl* varDecl = (aeNodeVarDecl*)clss->m_items[i];

            for (auto i : varDecl->m_decls)
            {
                aeField fieldInfo;
                fieldInfo.name = i.m_name;
                fieldInfo.size = varDecl->m_type.getSize();
                fieldInfo.type = varDecl->m_type;
                typeInfo->createField(fieldInfo);

                CompilerLog("REGISTERING TYPE VAR %s\n", i.m_name.c_str());
            }

        }
    }

    // Now the class is compiled, we generate constructor and destructor
    emitClassConstructors(typeInfo, clss);
    emitClassDestructors(typeInfo, clss);

    m_classes.pop_back();

    m_currentStruct = nullptr;

    return RzCompileResult::ok;
}

void RzCompiler::emitClassConstructors(RzType* classType, AEStructNode* classNode)
{
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
    m_reporter.emitLog("error C" + errorCode + ": " + message);

    if (m_currentFunction)
    {
        m_currentFunction->m_compiled = false; // Compilation failed
    }
}

RzCompileResult RzCompiler::emitBranchCode(aeNodeBranch* cond)
{
    aeNodeExpr* test_expr = cond->m_expression.get();
    aeNodeBlock* nested_code = cond->m_block.get();

    // The expression must evaluate first
    aeExprContext exprContext;
    exprContext.expectedResult = aeQualType(m_env->getTypeInfo("bool"));
    emitExpressionEval(test_expr, aeExprContext());

    // Now that the expression is evaluated and stored in a register, let's set the jmp
    emitInstruction(OP_JZ, 0, 0);
    int jmptestpc = m_cursor - 1;

    // Now let's define the actual block
    emitBlock(nested_code);

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
    push_scope();

    // Emit the code that initializes the control vars
    // They are automatically scoped and are destructed in the end of the loop
    emitStatement(forloop->initStatement.get());

    int pc_expreval = cursor(); // track the first instruction of this while, to jump back to evaluation each iteration

    aeExprContext ectx;
    ectx.must_be_rvalue = true;
    emitExpressionEval(forloop->expr.get(), ectx);

    // Evaluate the expression
    int jzInstrIndex = emitInstruction(OP_JZ, 0, 0, 0);

    emitBlock(forloop->block.get());

    // After the code executes, apply the update expression
    emitExpressionEval(forloop->incrExpr.get(), ectx);

    // After the block terminates normally, we always go back up to expression evaluation
    // when the expression fails, it will jump right away to after this
    emitInstruction(OP_JMP, pc_expreval - 1);

    // if the expression evaluates false, jump to after the for
    setinst_a(m_module->m_code[jzInstrIndex], cursor() - jzInstrIndex - 1);

    pop_scope();
    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::emitWhileLoop(aeNodeWhile* whileloop) {
    int pc_expreval = cursor();

    aeExprContext ectx;
    ectx.must_be_rvalue = true;
    emitExpressionEval(whileloop->expr.get(), ectx);

    int jmptestpc = emitInstruction(OP_JZ, 0, 0, 0);

    emitBlock(whileloop->block.get());

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

void RzCompiler::emitConstructorInjection(aeNodeFunction* node, AEFunction* function)
{
    // This is where the initialization is done, before any user constructor instruction is executed
    AEStructNode* cl = getTopClassNode();

    for (auto& field : cl->m_typeInfo->m_fields)
    {
        RZLOG("INIT %s\n", field.name.c_str());
        if (!field.type.isPod())
        {
            emitPushThis();
            emitInstruction(OP_LOADADDR, AEK_THIS, field.offset);

            if (field.type.isTemplated())
            {
                // Templated types get the type as parameter
                for (int i = 0; i < field.type.getNumTemplateArgs(); ++i)
                {
                    aeQualType targ = field.type.getTemplateArg(i);
                    emitInstruction(OP_TYPEINFO, m_env->getTypeInfoIndex(targ.m_type));
                }
            }

            aeNodeFunctionCall fnCall;
            fnCall.m_name = "array";
            emitFunctionCall(field.type, &fnCall, aeExprContext());

            RZLOG("INJECTED CONSTRUCTION %s\n", field.name.c_str());
        }
    }
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
        emitDebugTrace();
        auto ret = emitStatement(static_cast<AEStmtNode*>(codeblock->m_items[i]));
        if (ret == RzCompileResult::aborted)
        {
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

    case AEN_FUNCTIONCALL:
        return emitFunctionCall(aeQualType(), static_cast<aeNodeFunctionCall*>(stmt), aeExprContext());

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
            if (m_logExprStmt)
                emitDebugPrint("Evaluating " + stmt->str());

            return emitAssignOp(((aeNodeBinaryOperator*)stmt)->operandA, ((aeNodeBinaryOperator*)stmt)->operandB);
        }
        else {
            RZLOG("The operation is not allowed as a top level statement\n");
            return RzCompileResult::aborted;
        }
    }
    case AEN_ACCESSOPERATOR: {
        if (((aeNodeAccessOperator*)stmt)->m_b->m_nodeType == AEN_FUNCTIONCALL) {
            if (m_logExprStmt)
                emitDebugPrint("Evaluating " + stmt->str());

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
