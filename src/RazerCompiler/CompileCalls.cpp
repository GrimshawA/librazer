#include <RazerCompiler/aeCompiler.h>

AEFunction* AECompiler::selectFunction(aeNodeFunctionCall* fn)
{
	AEFunction* r = nullptr;

	if (fn->getParentExpression() && fn->getParentExpression()->m_nodeType == AEN_ACCESSOPERATOR)
	{
		// the fn belongs to some construct
		aeQualType leftSideType = ((aeNodeAccessOperator*)fn->getParentExpression())->m_a->getQualifiedType(this);
		std::string symbolName = leftSideType.getTypeName() + "." + fn->m_name;
		r = m_env->getFunctionByName(fn->m_name);
	}
	else
	{
		r = m_env->getFunctionByName(fn->m_name);
	}

	return r;
}


void AECompiler::emitFunctionCall(aeQualType beingCalledOn, aeNodeFunctionCall* fn, aeExprContext exprCtx)
{
	std::string finalSymbolName = fn->m_name;

	if (!beingCalledOn)
	{
		// This seems to be a "top-level" call, meaning it could have to be called on this, or a global func
		auto topClass = getTopClassNode();
		if (topClass && topClass->hasMethod(fn->m_name))
		{
			finalSymbolName = topClass->m_name + "." + fn->m_name;

			aeNodeFunction* calledMethod = topClass->getMethod(fn->m_name);
			if (!calledMethod->is_static)
			{
				//emitPushThis();
			}
		}
	}
	else
	{
		finalSymbolName = beingCalledOn.m_type->getSymbolName() + "." + fn->m_name;

	}

	CompilerLog("Function call %s\n", finalSymbolName.c_str());
	AEFunction* func = m_env->getFunctionByName(finalSymbolName);
	if (!func)
	{
		CompilerError("0002", "Calling an unknown function " + finalSymbolName);
		return;
	}

	// First allocate some room for the return value
	//emitInstruction(OP_MOV, fn->getReturnTypeSize());

	// Emit the arguments
	int i = 0;
	for (auto it = fn->m_args.rbegin(); it != fn->m_args.rend(); ++it)
	{
		aeExprContext arg_ctx;
		arg_ctx.rx_value = true;
		arg_ctx.expectedResult = fn->getArgType(i);
		emitExpressionEval((*it), arg_ctx);
	}


	int functionIndex = m_env->getFunctionIndexByName(finalSymbolName);

	if (functionIndex == -1)
		CompilerError("0002", "Could not find the function to call! " + finalSymbolName);

	if (func->m_native)
	{
		emitDebugPrint("NATIVE FUNCTION CALL " + finalSymbolName);
		emitInstruction(OP_CALLMETHOD_NAT, functionIndex);
	}
	else
	{
		emitInstruction(OP_CALL, functionIndex);
	}
}

void AECompiler::compileVariantCall(aeNodeExpr* lhs, aeNodeFunctionCall* fn)
{
	// Emit the arguments
	int i = 0;
	for (auto it = fn->m_args.rbegin(); it != fn->m_args.rend(); ++it)
	{
		aeExprContext arg_ctx;
		arg_ctx.rx_value = true;
		arg_ctx.expectedResult = fn->getArgType(i);
		emitExpressionEval((*it), arg_ctx);
	}

	/// Push the variant this function was called on
	emitExpressionEval(lhs, aeExprContext());

	// Calls a function on a dynamic variable
	int fnNameIndex = m_module->identifierPoolIndex(fn->m_name);
	emitInstruction(OP_VARCALL, fnNameIndex);
}

void AECompiler::emitLateBoundCall(aeNodeFunctionCall* fn)
{
	// Calls a function on a static object, if it supports it
}