#include <RazerCompiler/RzCompiler.h>
#include <RazerCompiler/TypeResolver.h>
#include <Logger.h>

RzFunction* RzCompiler::selectFunction(aeNodeFunctionCall* fn) {
    RzFunction* r = nullptr;

    if (fn->getParentExpression() && fn->getParentExpression()->m_nodeType == AEN_ACCESSOPERATOR)
    {
        // the fn belongs to some construct
        RzQualType leftSideType = resolveQualifiedType(*this, *((aeNodeAccessOperator*)fn->getParentExpression())->m_a);
        std::string symbolName = leftSideType.getTypeName() + "." + fn->m_name;
       // r = m_env->getFunctionByName(fn->m_name);
    }
    else
    {
       // r = m_env->getFunctionByName(fn->m_name);
    }

    return r;
}


RzCompileResult RzCompiler::emitFunctionCall(aeNodeExpr& selfExpr, RzQualType beingCalledOn, aeNodeFunctionCall* fn, RzExprContext exprCtx) {
    std::string finalSymbolName = fn->m_name;

    if (fn->m_name == "break2")
	{
    	emitBreakpoint();
    	// Short circuit for an intrinsic
    	return RzCompileResult::OK;
	}

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
                beingCalledOn = topClass->m_typeInfo;
            }
        }
    }
    else
    {
        finalSymbolName = beingCalledOn.m_type->getSymbolName() + "." + fn->m_name;

    }

    if (beingCalledOn) {
        return compileStaticObjectCall(selfExpr, beingCalledOn, *fn);
    }
    else {
        RZLOG("Don't know how to call global functions yet.");
        return RzCompileResult::aborted;
    }
}

void RzCompiler::compileVariantCall(aeNodeExpr* lhs, aeNodeFunctionCall* fn) {
    // Emit the arguments
    int i = 0;
    for (auto it = fn->m_args.rbegin(); it != fn->m_args.rend(); ++it)
    {
        RzExprContext arg_ctx;
        arg_ctx.rx_value = true;
        arg_ctx.expectedResult = fn->getArgType(i);
        emitExpressionEval((*it), arg_ctx);
    }

    /// Push the variant this function was called on
    emitExpressionEval(lhs, RzExprContext());

    // Calls a function on a dynamic variable
    int fnNameIndex = m_module->identifierPoolIndex(fn->m_name);
    emitInstruction(OP_VARCALL, fnNameIndex);
}

void RzCompiler::emitLateBoundCall(aeNodeFunctionCall* fn) {
    // Calls a function on a static object, if it supports it
}

RzCompileResult RzCompiler::compileStaticObjectCall(aeNodeExpr& selfExpr, RzQualType obj, aeNodeFunctionCall& call) {


    RzType* typeInfo = obj.getType();
    if (!typeInfo) {
        RZLOG("Calling function on unknown type");
        return RzCompileResult::aborted;
    }

    RzType::MethodInfo method = typeInfo->selectMethod(call.m_name, std::vector<RzQualType>());

    if (method.name.empty())
    {
        RZLOG("error: '%s' is not a method of '%s'\n", call.m_name.c_str(), typeInfo->getName().c_str());
        return RzCompileResult::aborted;
    }

    if (method.args.size() != call.m_args.size()) {
        RZLOG("error: arguments list differ for '%s'. Expects %d, %d provided\n", call.m_name.c_str(), method.args.size(), call.m_args.size());
        return RzCompileResult::aborted;
    }

    if (method.native) {

        // Native calls convention (with generic)
        // Push the arguments right to left
        // Finalize with this pointer as the last push "fn(self, arg0, arg1, ..)"

        RZLOG("METHOD %s %d args\n", method.name.c_str(), method.args.size());
        auto argsResult = compileArgsPush(call.m_args, method.args);
        if (argsResult == RzCompileResult::aborted)
            return argsResult;

        RzExprContext arg_ctx;
        arg_ctx.rx_value = true;
        emitExpressionEval(&selfExpr, arg_ctx);

        return compileNativeObjectCall(typeInfo->getModule()->index(), method);
    }
    else {
        auto argsResult = compileArgsPush(call.m_args, method.args);
        if (argsResult == RzCompileResult::aborted)
            return argsResult;

        emitPushThis();

        int moduleIndex = m_module->index();
        int functionIndex = typeInfo->getFunctionId(call.m_name);
        emitInstruction(OP_CALL, moduleIndex, functionIndex);
    }

    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::compileNativeObjectCall(int moduleIndex, RzType::MethodInfo info) {
    emitInstruction(OP_NATIVECALL, moduleIndex, info.offset);
    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::compileArgsPush(const std::vector<aeNodeExpr*>& args, const std::vector<RzQualType>& expectedTypes) {
    if (args.empty())
        return RzCompileResult::ok;

    if (args.size() != expectedTypes.size()) {
        RZLOG("error: Argument count differs. %d provided. %d expected\n", args.size(), expectedTypes.size());
        return RzCompileResult::aborted;
    }

    int i = 0;
    for (auto it = args.rbegin(); it != args.rend(); ++it)
    {
        RzQualType t = buildQualifiedType(*it);
        RzQualType expectedT = expectedTypes[i];

        RzCompileResult r = emitExpressionEval((*it), RzExprContext::temporaryRValue());
        if (r == RzCompileResult::aborted)
            return r;

        // Might need to convert to fullfill the function requirements
        if (t != expectedT) {
            r = implicitConvert(t, expectedT);
            if (r == RzCompileResult::aborted) {
                RZLOG("error: Expected argument type doesn't match the provided one.\n"
                      "expected: %s; passed: %s\n", expectedT.str().c_str(), t.str().c_str());
                return RzCompileResult::aborted;
            }
        }

        ++i;
    }
    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::pushImplicitThis(aeNodeExpr& expr) {


    return RzCompileResult::ok;
}
