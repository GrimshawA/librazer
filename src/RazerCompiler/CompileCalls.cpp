#include <RazerCompiler/RzCompiler.h>
#include <Logger.h>

AEFunction* RzCompiler::selectFunction(aeNodeFunctionCall* fn)
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


RzCompileResult RzCompiler::emitFunctionCall(aeQualType beingCalledOn, aeNodeFunctionCall* fn, aeExprContext exprCtx)
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

    if (beingCalledOn) {
        return compileStaticObjectCall(beingCalledOn, *fn);
    }
    else {
        RZLOG("Don't know how to call global functions yet.");
        return RzCompileResult::aborted;
    }
}

void RzCompiler::compileVariantCall(aeNodeExpr* lhs, aeNodeFunctionCall* fn)
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

void RzCompiler::emitLateBoundCall(aeNodeFunctionCall* fn)
{
	// Calls a function on a static object, if it supports it
}

RzCompileResult RzCompiler::compileStaticObjectCall(aeQualType obj, aeNodeFunctionCall& call) {

    RzType* typeInfo = obj.getType();
    if (!typeInfo) {
        RZLOG("Calling function on unknown type");
        return RzCompileResult::aborted;
    }



    AEFunction* func = typeInfo->getFunction(call.m_name);
    if (!func)
    {
        RZLOG("error: '%s' is not a method of '%s'\n", call.m_name.c_str(), typeInfo->getName().c_str());
        return RzCompileResult::aborted;
    }

    // First allocate some room for the return value
    //emitInstruction(OP_MOV, fn->getReturnTypeSize());

    // Emit the arguments
    int i = 0;
    for (auto it = call.m_args.rbegin(); it != call.m_args.rend(); ++it)
    {
        aeExprContext arg_ctx;
        arg_ctx.rx_value = true;
        arg_ctx.expectedResult = call.getArgType(i);
        emitExpressionEval((*it), arg_ctx);
    }


    //int functionIndex = m_env->getFunctionIndexByName(finalSymbolName);

    /*if (functionIndex == -1)
        CompilerError("0002", "Could not find the function to call! " + finalSymbolName);

    if (func->m_native)
    {
        emitDebugPrint("NATIVE FUNCTION CALL " + finalSymbolName);
        emitInstruction(OP_CALLMETHOD_NAT, functionIndex);
    }
    else
    {
        emitInstruction(OP_CALL, functionIndex);
    }*/

    return RzCompileResult::ok;
}
