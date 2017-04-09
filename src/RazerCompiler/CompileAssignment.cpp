#include <RazerCompiler/RzCompiler.h>
#include <Logger.h>

RzCompileResult RzCompiler::emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs)
{
	aeExprContext ectx;
	ectx.must_be_rvalue = true;

	aeQualType T1 = buildQualifiedType(lhs);
	aeQualType T2 = buildQualifiedType(rhs);

    if (!T1.m_typeString.empty() && !T1.m_type) {
        // TODO: Do it right
        T1.m_type = m_env->getTypeInfo(T1.m_typeString);
    }

    if (!T2) {
        RZLOG("Unknown type right of the assignment: '%s'\n" , T2.m_typeString.c_str());
        return RzCompileResult::aborted;
    }

	RZLOG("T1 %s T2 %s\n", T1.str().c_str(), T2.str().c_str());

	if (!T1.isVariant() && (T1.getType() != T2.getType()) && !canImplicitlyConvert(T2, T1))
	{
		CompilerError("0002", "Cannot convert from " + T2.str() + " to " + T1.str());
        return RzCompileResult::aborted;
	}

	if (T1.isVariant())	{
        return compileVarAssign(lhs, rhs);
	}
    else {
        return compileStaticAssign(*lhs, *rhs);
    }
}

RzCompileResult RzCompiler::compileVarAssign(aeNodeExpr* lhs, aeNodeExpr* rhs)
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
		// Assigning an object to a variant, need to identify its type
		int moduleIndex = m_module->resolveTypeModuleIndex(rhsType.getType());
		if (moduleIndex == -1)
		{
			RZLOG("Compiler error. Unresolved module\n");
            return RzCompileResult::aborted;
		}

		int typeIndex;

		if (moduleIndex == 0)
			typeIndex = m_module->getTypeIndex(rhsType.getType());
		else
			typeIndex = m_module->getDependantModule(moduleIndex)->getTypeIndex(rhsType.getType());

		emitInstruction(OP_VARSTORE, AE_VARIANTTYPE_OBJECT, moduleIndex, typeIndex);
	}

    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::compileStaticAssign(aeNodeExpr& lhs, aeNodeExpr& rhs) {

    // Left hand gets loaded (Address of it)
    emitLoadAddress(&lhs);

    // Right hand gets loaded (value)
    aeExprContext exprContext;
    exprContext.expectedResult = buildQualifiedType(&lhs);
    emitExpressionEval(&rhs, aeExprContext());

    // Convert if required
    /*if ((T1.getType() != T2.getType()) && canImplicitlyConvert(T2, T1))
    {
        m_typeSystem.performConversion(T2, T1, this);
    }*/

    // Generate actual assignment
    int assignType = -1;
    if (1)
    {
        assignType = AEP_PTR;
    }
    else
    {
        assignType = AEP_INT32;
    }

    emitDebugPrint("OP_SET gen " + std::to_string(assignType));

    // Finalize the assignment
    emitInstruction(OP_SET, 0, 0, assignType);

    return RzCompileResult::ok;
}

