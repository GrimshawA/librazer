#include <RazerCompiler/RzCompiler.h>
#include <RazerVM/InstructionSet.h>
#include <RazerVM/VirtualMachine.h>
#include <RazerRuntime/RzEngine.h>
#include <RazerCompiler/TypeResolver.h>
#include <RazerCompiler/OverloadResolver.h>

#include <Logger.h>

#include <cassert>

RzCompileResult RzCompiler::emitExpressionEval(aeNodeExpr* expr, RzExprContext exprContext)
{
	if (expr->m_nodeType == AEN_IDENTIFIER)
	{
		// Load the var address into the stack
		emitVarExpr(static_cast<aeNodeIdentifier*>(expr), exprContext);
	}
	else if (expr->m_nodeType == AEN_FUNCTIONCALL)
	{
        return emitFunctionCall(aeNodeExpr(), RzQualType(), static_cast<aeNodeFunctionCall*>(expr), RzExprContext());
	}
	else if (expr->m_nodeType == AEN_ACCESSOPERATOR)
	{
        return emitMemberOp(static_cast<aeNodeAccessOperator*>(expr));
	}
	else if (expr->m_nodeType == AEN_INTEGER || expr->m_nodeType == AEN_STRING || expr->m_nodeType == AEN_FLOAT)
	{
		emitLoadLiteral((aeNodeLiteral*)expr);
	}
	else if (expr->m_nodeType == AEN_BINARYOP)
	{
		aeNodeBinaryOperator* binaryop = static_cast<aeNodeBinaryOperator*>(expr);
		if (binaryop->oper == ">" || binaryop->oper == "<")
		{
            emitBinaryOp(binaryop);
		}
		else if (binaryop->isArithmetic())
		{
			emitBinaryOp(binaryop);
		}
		else if (binaryop->oper == "=")
		{
            return emitAssignOp(binaryop->operandA, binaryop->operandB);
		}
	}
	else if (expr->m_nodeType == AEN_NEW)
	{
        RzCompileResult res = compileNew((aeNodeNew&)*expr);
        return res;
	}

    return RzCompileResult::ok;
}

void RzCompiler::emitPushThis()
{
    // Find where in the stack frame the this ptr is and load it from there
    // It means to load stack memory relative to the base pointer, found
    // at offset the this resides in, plus its size (stack grows backwards)
    VariableStorageInfo var = getVariable("this");
    emitInstruction(OP_LOAD, AEK_EBP, var.offset + sizeof(void*), AEP_PTR);
}

void RzCompiler::loadVarRef(aeNodeExpr* e)
{
	if (e->m_nodeType == AEN_ACCESSOPERATOR)
	{
		aeNodeAccessOperator* op = (aeNodeAccessOperator*)e;
		loadVarRef(op->m_a);

		if (op->m_b->m_nodeType == AEN_IDENTIFIER)
		{
			emitInstruction(OP_VARLOADREF, 0, m_module->identifierPoolIndex(static_cast<aeNodeIdentifier*>(op->m_b)->m_name));
		}
	}
	else if (e->m_nodeType == AEN_IDENTIFIER)
	{
		aeNodeIdentifier* ident = (aeNodeIdentifier*)e;
		if (getVariable(ident->m_name).mode == AE_VAR_LOCAL)
		{
			emitInstruction(OP_VARLOADREF, 1, getVariable(ident->m_name).offset);
		}
	}
}

void RzCompiler::emitBinaryOp(aeNodeBinaryOperator* operation)
{
	if (m_logExprOps)
		emitDebugPrint("OP " + operation->oper + " " + operation->operandA->str() + " " + operation->operandB->str());

	if (operation->oper == "=")
	{
		emitAssignOp(operation->operandA, operation->operandB);
	}
	if (operation->oper == "+" || operation->oper == "-" || operation->oper == "*" || operation->oper == "/")
	{
        emitArithmeticOp(operation, RzExprContext());
	}
	if (operation->isRelational())
	{
        emitArithmeticOp(operation, RzExprContext());
	}
}

RzCompileResult RzCompiler::emitArithmeticOp(aeNodeBinaryOperator* op, const RzExprContext& context)
{
    if (!op->operandA || !op->operandB) {
        assert("error: parser issue. binary expression is ill formed\n");
        return RzCompileResult::aborted;
    }

    RzQualType lhsType = resolveQualifiedType(*this, *op->operandA);
    RzQualType rhsType = resolveQualifiedType(*this, *op->operandB);
    RzQualType effectiveOperationType;

    if (!lhsType.sameTypeAs(rhsType)) {
        // Conversion could be required

        if (lhsType.isPrimitive() && rhsType.isPrimitive()) {
            // Different primitive types are a case for promotion
            // Both sides of the expression need to be converted into promotedType

            effectiveOperationType = resolvePromotedType(lhsType, rhsType);

            emitExpressionEval(op->operandA, RzExprContext::temporaryRValue());
            implicitConvert(lhsType, effectiveOperationType);

            emitExpressionEval(op->operandB, RzExprContext::temporaryRValue());
            implicitConvert(rhsType, effectiveOperationType);
        }
    }
    else {
        effectiveOperationType = lhsType;
        emitExpressionEval(op->operandA, RzExprContext::temporaryRValue());
        emitExpressionEval(op->operandB, RzExprContext::temporaryRValue());
    }

    int primitiveId = resolvePrimitiveConstantFromType(effectiveOperationType);

    if (op->oper == "+") {
        emitInstruction(OP_ADD, primitiveId);
	}

    if (op->oper == "-") {
        emitInstruction(OP_SUB, primitiveId);
	}

    if (op->oper == "*") {
        emitInstruction(OP_MUL, primitiveId);
	}

    if (op->oper == "/") {
        emitInstruction(OP_DIV, primitiveId);
	}

    if (op->oper == ">")
    {
        emitInstruction(OP_GT, primitiveId);
    }

    if (op->oper == "<") {
        emitInstruction(OP_LT, primitiveId);
    }

    if (op->oper == ">=") {
        emitInstruction(OP_GTE, primitiveId);
    }

    if (op->oper == "<=") {
        emitInstruction(OP_LTE, primitiveId);
    }

    if (op->oper == "==") {
        emitInstruction(OP_EQ, primitiveId);
    }

    if (op->oper == "!=") {
        emitInstruction(OP_NEQ, primitiveId);
    }
}

bool RzCompiler::canConvertType(RzType* typeA, RzType* typeB)
{
	return false;
}

RzCompileResult RzCompiler::implicitConvert(RzQualType from, RzQualType to)
{
    m_typeSystem.performConversion(from, to, this);
    return RzCompileResult::ok;
}

RzCompileResult RzCompiler::emitLoadAddress(aeNodeExpr* expr)
{
	if (expr->m_nodeType != AEN_IDENTIFIER)
	{
        RZLOG("error: emitLoadAddress: Only know how to load a variable ref\n");
        return RzCompileResult::aborted;
	}
	aeNodeIdentifier* varExpr = (aeNodeIdentifier*)expr;

	auto varStorage = getVariable(varExpr->m_name);

	if (varStorage.mode == AE_VAR_LOCAL)
	{
		// Load a local variable address into the stack
		emitInstruction(OP_LOADADDR, AEK_EBP, varStorage.offset);
		CompilerLog("Loading local\n");
	}
	else if (varStorage.mode == AE_VAR_FIELD)
	{
        RzCompileResult r = loadMemberAddress(varExpr->m_name);
        if (r == RzCompileResult::aborted)
            return r;
	}
	else if (varStorage.mode == AE_VAR_GLOBAL)
	{
		CompilerLog("globals not working\n");
	}
	else
	{
		// The variable could not be found
        RZLOG("The referenced variable cannot be found\n");
        return RzCompileResult::aborted;
	}
}

void RzCompiler::emitLoadLiteral(aeNodeLiteral* lt) {
	// Always leaves temporary rvalues in the stack

    if (lt->m_nodeType == AEN_INTEGER) {
		aeNodeInteger* integer_node = (aeNodeInteger*)lt;
		emitInstruction(OP_LOADK, AEK_INT, m_env->getIntegerLiteral(integer_node->value), 4);
	}
    else if (lt->m_nodeType == AEN_FLOAT) {
		aeNodeFloat* literal = (aeNodeFloat*)lt;
		emitInstruction(OP_LOADK, AEK_FLOAT, m_env->getFloatLiteral(literal->value), 4);
	}
    else if (lt->m_nodeType == AEN_STRING) {
		aeNodeString* literal = (aeNodeString*)lt;
		emitInstruction(OP_LOADK, AEK_STRING, m_env->getStringLiteral(literal->value), 4);
	}
}

RzCompileResult RzCompiler::loadMemberVariable(const std::string& name) {
    auto var = getVariable(name);

    emitPushThis();

    if (!var.type.m_type) {
        RZLOG("error: Cannot load member variable with unknown type '%s'\n", var.type.m_typeString.c_str());
        return RzCompileResult::aborted;
    }

    // Primitive types are handled directly with value semantics
    // Otherwise, types are treated as object references
    // loaded as a plain pointer

    if (var.type.getType()->getName() == "int32") {
        emitInstruction(OP_LOAD, AEK_THIS, var.offset, AEP_INT32);
        return RzCompileResult::ok;
    }
    else if (var.type.getType()->getName() == "float") {
        emitInstruction(OP_LOAD, AEK_THIS, var.offset, AEP_FLOAT);
        return RzCompileResult::ok;
    }
    else if (var.type.getType()->getName() == "boolean") {
        emitInstruction(OP_LOAD, AEK_THIS, var.offset, AEP_INT32);
        return RzCompileResult::ok;
    }
    else {
        emitInstruction(OP_LOAD, AEK_THIS, var.offset, AEP_PTR);
        return RzCompileResult::ok;
    }
}

RzCompileResult RzCompiler::loadMemberAddress(const std::string& name) {
    auto var = getVariable(name);
    emitPushThis();

    if (!var.type.m_type) {
        RZLOG("error: Cannot load member variable with unknown type '%s'\n", var.type.m_typeString.c_str());
        return RzCompileResult::aborted;
    }

    // Load the address of a variable for writing to
    // Primitives are handled as special cases
    // all object references are loaded as a raw pointer

    if (var.type.getType()->getName() == "int32") {
         emitInstruction(OP_LOADADDR, AEK_THIS, var.offset);
         return RzCompileResult::ok;
    }
    else if (var.type.getType()->getName() == "float") {
        emitInstruction(OP_LOADADDR, AEK_THIS, var.offset);
        return RzCompileResult::ok;
    }
    else if (var.type.getType()->getName() == "boolean") {
        emitInstruction(OP_LOADADDR, AEK_THIS, var.offset);
        return RzCompileResult::ok;
    }
    else {
         emitInstruction(OP_LOADADDR, AEK_THIS, var.offset);
         return RzCompileResult::ok;
    }
}

RzCompileResult RzCompiler::emitMemberOp(aeNodeAccessOperator* acs)
{
	/**
		We are compiling a.b
		b must be a part of a so it can be used
	*/

	//printf("Compiling operator.()\n");

	RzQualType Ta = buildQualifiedType(acs->m_a);
	if (!Ta)
	{
        RZLOG("Cannot find the type of \n");
        return RzCompileResult::aborted;
	}

	// When the left side evaluates to a variant and the right side is a call
	// we handle it in a special way
	if (acs->m_b->m_nodeType == AEN_FUNCTIONCALL && Ta.getName() == "var")
	{
		compileVariantCall(acs->m_a, static_cast<aeNodeFunctionCall*>(acs->m_b));
        return RzCompileResult::ok;
	}

    RzExprContext exprContext;
	exprContext.expectedResult = Ta;
    if (1) // always loaded as a ptr for now
		exprContext.rx_value = true; // for handles we want to load their value
	else
		exprContext.lvalue = true; // for values we want to get them loaded for edit
    //emitExpressionEval(acs->m_a, exprContext);

	if (acs->m_b->m_nodeType == AEN_FUNCTIONCALL)
	{
		CompilerLog("Calling function on %s\n", Ta.str().c_str());

		// Emit the appropriate calling code, which assumes the arguments and obj to call on are pushed already
        return emitFunctionCall(*acs->m_a, Ta, static_cast<aeNodeFunctionCall*>(acs->m_b), RzExprContext());
	}
	else
	{
		if (acs->m_b->m_nodeType == AEN_IDENTIFIER && Ta.getType()->isEnum())
		{
			aeNodeIdentifier* refNode = (aeNodeIdentifier*)acs->m_b;
			emitEnumValue((aeEnum*)Ta.getType(), refNode->m_name);
		}
		else if (acs->m_b->m_nodeType == AEN_IDENTIFIER && Ta.str() == "var")
		{
			emitInstruction(OP_VARLOAD, m_module->identifierPoolIndex(((aeNodeIdentifier*)acs->m_b)->m_name));
		}
	}
    return RzCompileResult::ok;
}

void RzCompiler::emitConditionalOp(aeNodeBinaryOperator* operation)
{
	/*
		Operators: >, >=, <, <=, ==, !=, &&, ||

		These operators are handled together as they follow similar behavior.
		The result of the operation is put on the stack unless the expression
		context denies it.
		The result is either true or false, represented as bool.
	*/
    assert(false);
}

RzCompileResult RzCompiler::emitVarExpr(aeNodeIdentifier* var, const RzExprContext& parentExprContext)
{
	/// The variable needs to be loaded into the stack, as it will be used to evaluate an expression
	auto varInfo = getVariable(var->m_name);

	int loadFrom = AEK_EBP;
	int offsetOnRefFrame = varInfo.offset;

    if (varInfo.mode == AE_VAR_FIELD && parentExprContext.rx_value) {
        return loadMemberVariable(var->m_name);
    }
    else if (varInfo.mode == AE_VAR_FIELD && parentExprContext.lvalue) {
        return loadMemberAddress(var->m_name);
    }
	
	else if (varInfo.mode == AE_VAR_LOCAL)
	{
		if (varInfo.type.str() == "var"){
			emitInstruction(OP_PUSHVAR, varInfo.offset);
            RZLOG(" Loading var: %s offset %d\n", varInfo.name.c_str(), varInfo.offset);
            return RzCompileResult::ok;
		}
	}

	if (parentExprContext.rx_value)
	{
		int kind = AEP_INT32;

		if (parentExprContext.expectedResult.m_handle)
			kind = AEP_PTR;

		emitInstruction(OP_LOAD, loadFrom, offsetOnRefFrame, kind);
	}
	else if (parentExprContext.lvalue)
	{
		emitInstruction(OP_LOADADDR, loadFrom, offsetOnRefFrame);
	}

    return RzCompileResult::ok;
}

void RzCompiler::emitSubscriptOp(aeNodeSubscript* subscript)
{

}
