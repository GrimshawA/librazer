#include <RazerCompiler/RzCompiler.h>
#include <RazerCompiler/Errors.h>
#include <RazerCompiler/BuildReport.h>
#include <RazerCompiler/TypeResolver.h>
#include <razer/utils/Logger.h>

RzCompileResult RzCompiler::compileVarDecl(const aeNodeVarDecl& varDecl)
{
	/*
		Compiles a declaration within a code scope.
		Includes stack space allocation, object preparation, constructor calling etc.
	*/

    if (varDecl.m_decls.size() != 1) {
        RZLOG("error: only one variable allowed in var declaration");
        return RzCompileResult::aborted;
    }

    RzQualType declType = varDecl.m_type;

	if (declType.getName() == "var")
	{
		for (int i = 0; i < varDecl.m_decls.size(); ++i)
		{
			compileVariantStackAlloc(varDecl.m_decls[i].m_name, varDecl.m_decls[i].m_init);
		}
        return RzCompileResult::ok;
	}

    declType = RzQualType(m_env->getTypeInfo(varDecl.m_type.m_typeString));

	if (!declType.getType())
	{
        rzcerr_unknown_type(varDecl.m_type.m_typeString);
        RZLOG("error: decl boom\n");
        return RzCompileResult::aborted;
	}

    RzType* varType = declType.m_type;
	auto& scope = m_scopes[m_scopes.size() - 1];

    // Ensure the variable is not already declared
    if (getVariable(varDecl.m_decls[0].m_name).mode == AE_VAR_LOCAL) {

        RZLOG("error: Local variable redefinition '%s'\n", varDecl.m_decls[0].m_name.c_str());
        return RzCompileResult::aborted;
    }

    declareStackVar(varDecl.m_decls[0].m_name, declType);

	emitInstruction(OP_MOV, AEK_ESP, -(int)varType->getSize());

	// Now the variable is initialized and part of its scope, initialize it
	if (varDecl.m_decls[0].m_init)
	{
//		if (m_logAllocs)
//            emitDebugPrint("Evaluating " + varDecl.m_decls[0].m_init->str());

        RzCompileResult r = emitExpressionEval(varDecl.m_decls[0].m_init, RzExprContext());
        if (r.m_status == RzCompileResult::ABORTED)
            return r;
	}

    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::compileVariantStackAlloc(const std::string& identifier, aeNodeExpr* initExpr)
{
	declareStackVar(identifier, m_env->getTypeInfo("var"));
	emitInstruction(OP_PUSHVAR);

	if (initExpr)
	{
		// There is a right hand side to assign, compile, cast and set
		aeNodeIdentifier ident;
		ident.m_name = identifier;
		compileVarAssign(&ident, static_cast<aeNodeBinaryOperator*>(initExpr)->operandB);
	}

	return RzCompileResult();
}

RzCompileResult RzCompiler::compileNew(aeNodeNew& newExpr)
{
	// new X() expression must generate a new object instance.

    RZLOG("NEW: %s\n", newExpr.str().c_str());

    auto typeToAllocate = resolveQualifiedType(*this, *newExpr.newExpr);


    newExpr.m_instanceType = typeToAllocate;
    //newExpr.m_instanceType = RzQualType(m_env->getTypeInfo(newExpr.type));

	if (newExpr.m_instanceType.getType() == nullptr)
	{
//		m_report->emitCompilerError(std::string("new: unknown type ") + newExpr.m_instanceType.str().c_str());
		RZLOG("Compiler internal error. Unresolved type\n");
        return RzCompileResult(RzCompileResult::ABORTED);
	}
	
	int moduleIndex = m_module->resolveTypeModuleIndex(newExpr.m_instanceType.getType());
	if (moduleIndex == -1)
	{
		RZLOG("Compiler error. Unresolved module\n");
        return RzCompileResult(RzCompileResult::ABORTED);
	}

	int typeIndex;
	if (moduleIndex == 0)
		typeIndex = m_module->getTypeIndex(newExpr.m_instanceType.getType());
	else
		typeIndex = m_module->getDependantModule(moduleIndex)->getTypeIndex(newExpr.m_instanceType.getType());

	emitInstruction(OP_NEW, moduleIndex, typeIndex);

	if (!newExpr.m_instanceType.getType()->is_native)
	{
		// We just allocated room for the in-language type, we must now invoke its constructor.
		int constructorIndex = newExpr.m_instanceType.getType()->getConstructorIndex();
		int moduleIndex = m_module->m_context->getTypeModuleIndex(newExpr.m_instanceType.getType());
		emitInstruction(OP_CALL, moduleIndex, constructorIndex);
	}

	RZLOG("Compiled new %d %d\n\n", moduleIndex, typeIndex);

    return RzCompileResult(RzCompileResult::OK);
}
