#include <RazerCompiler/AECompiler.h>
#include <RazerCompiler/Errors.h>
#include <Logger.h>
#include <BuildReport.h>

RzCompileResult AECompiler::compileVarDecl(const aeNodeVarDecl& varDecl)
{
	/*
		Compiles a declaration within a code scope.
		Includes stack space allocation, object preparation, constructor calling etc.
	*/

	aeQualType declType = varDecl.m_type;

	if (declType.getName() == "var")
	{
		for (int i = 0; i < varDecl.m_decls.size(); ++i)
		{
			compileVariantStackAlloc(varDecl.m_decls[i].m_name, varDecl.m_decls[i].m_init);
		}
		return RzCompileResult();		
	}

   // declType = aeQualType(m_env->getTypeInfo(newExpr.type));

	if (!declType.getType())
	{
        rzcerr_unknown_type("TYPEZ");
		return RzCompileResult();
	}

	RzType* varType = varDecl.m_type.m_type;
	auto& scope = m_scopes[m_scopes.size() - 1];

	declareStackVar(varDecl.m_decls[0].m_name, varDecl.m_type);


	emitInstruction(OP_MOV, AEK_ESP, -(int)varType->getSize());

	// Now the variable is initialized and part of its scope, initialize it
	if (varDecl.m_decls[0].m_init)
	{
		if (m_logAllocs)
			emitDebugPrint("Evaluating " + varDecl.m_decls[0].m_init->str());

		emitExpressionEval(varDecl.m_decls[0].m_init, aeExprContext());
	}
	else
	{
		// Default initialization
		if (varType->is_native)
		{
			emitInstruction(OP_LOAD, AEK_ESP);
			emitInstruction(OP_CALLMETHOD_NAT, m_env->getNativeBehaviorIndex(varType->getName(), "f"));
		}
	}

	return RzCompileResult();
}

RzCompileResult AECompiler::compileVariantStackAlloc(const std::string& identifier, aeNodeExpr* initExpr)
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

void AECompiler::compileNew(aeNodeNew& newExpr)
{
	// new X() expression must generate a new object instance.
	//int typeId = newExpr.m_instanceType

	newExpr.m_instanceType = aeQualType(m_env->getTypeInfo(newExpr.type));

	if (newExpr.m_instanceType.getType() == nullptr)
	{
		m_report->emitCompilerError(std::string("new: unknown type ") + newExpr.m_instanceType.str().c_str());
		RZLOG("Compiler internal error. Unresolved type\n");
		return;
	}
	
	int moduleIndex = m_module->resolveTypeModuleIndex(newExpr.m_instanceType.getType());
	if (moduleIndex == -1)
	{
		RZLOG("Compiler error. Unresolved module\n");
		return;
	}

	int typeIndex;
	if (moduleIndex == 0)
		typeIndex = m_module->getTypeIndex(newExpr.m_instanceType.getType());
	else
		typeIndex = m_module->getDependantModule(moduleIndex)->getTypeIndex(newExpr.m_instanceType.getType());

	emitInstruction(OP_NEW, moduleIndex, typeIndex);

	RZLOG("Compiled new %d %d\n\n", moduleIndex, typeIndex);
}
