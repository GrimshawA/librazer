#include <AEON/Compiler/AECompiler.h>

void AECompiler::emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs)
{
	aeExprContext ectx;
	ectx.must_be_rvalue = true;

	aeQualType T1 = buildQualifiedType(lhs);
	aeQualType T2 = buildQualifiedType(rhs);

	printf("T1 %s T2 %s\n", T1.str().c_str(), T2.str().c_str());

	if (!T1.isVariant() && (T1.getType() != T2.getType()) && !canImplicitlyConvert(T2, T1))
	{
		CompilerError("0002", "Cannot convert from " + T2.str() + " to " + T1.str());
		return;
	}

	if (T1.isVariant())
	{
		compileVarAssign(lhs, rhs);
		return;
	}

	// Left hand gets loaded (Address of it)
	emitLoadAddress(lhs);

	// Right hand gets loaded (value)
	aeExprContext exprContext;
	exprContext.expectedResult = buildQualifiedType(lhs);
	emitExpressionEval(rhs, exprContext);

	if ((T1.getType() != T2.getType()) && canImplicitlyConvert(T2, T1))
	{
		m_typeSystem.performConversion(T2, T1, this);
	}


	int assignType = -1;
	emitDebugPrint("ASSIGNING TO " + exprContext.expectedResult.str());
	if (exprContext.expectedResult.m_handle)
	{
		assignType = AEP_PTR;
	}
	else
	{
		assignType = AEP_INT32;
	}

	// Finalize the assignment
	emitInstruction(OP_SET, 0, 0, assignType);

}

void AECompiler::compileVarAssign(aeNodeExpr* lhs, aeNodeExpr* rhs)
{
	aeQualType varType = m_env->getTypeInfo("var");
	aeQualType rhsType = buildQualifiedType(rhs);

	loadVarRef(lhs);
	emitExpressionEval(rhs, aeExprContext());

	if (rhsType.getTypeName() == "int32")
	{
		emitInstruction(OP_VARSTORE, AE_VARIANTTYPE_INT);
	}
	else if (rhsType.getTypeName() == "var")
	{
		// Assigning var to var
		emitInstruction(OP_VARSTORE, AE_VARIANTTYPE_VAR);
	}
	else
	{
		// Fallback to object, not fully correct outcome
		emitInstruction(OP_VARSTORE, AE_VARIANTTYPE_OBJECT, 0, 0);
	}
}
