#include <AEON/Compiler/AECompiler.h>

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

	if (!declType.getType())
	{
		CompilerError("0003", "Declared local variable with unknown type.");
		return RzCompileResult();
	}

	AEType* varType = varDecl.m_type.m_type;
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
	
	//m_module->resolveTypeModuleIndex(newExpr.m_instanceType);

	emitInstruction(OP_NEW, 0, 0);
}