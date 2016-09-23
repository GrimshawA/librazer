#include <AEON/Compiler/aeCompiler.h>
#include <AEON/Runtime/AEByteCode.h>
#include <AEON/VM/AEVm.h>
#include <AEON/AEContext.h>

#include <cassert>

void AECompiler::emitExpressionEval(aeNodeExpr* expr, aeExprContext exprContext)
{
	if (expr->m_nodeType == AEN_IDENTIFIER)
	{
		// Load the var address into the stack
		emitVarExpr(static_cast<aeNodeIdentifier*>(expr), exprContext);
	}
	else if (expr->m_nodeType == AEN_FUNCTIONCALL)
	{
		emitFunctionCall(aeQualType(), static_cast<aeNodeFunctionCall*>(expr), aeExprContext());
	}
	else if (expr->m_nodeType == AEN_ACCESSOPERATOR)
	{
		emitMemberOp(static_cast<aeNodeAccessOperator*>(expr));
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
			emitConditionalOp(binaryop);
		}
		else if (binaryop->isArithmetic())
		{
			emitBinaryOp(binaryop);
		}
		else if (binaryop->oper == "=")
		{
			emitAssignOp(binaryop->operandA, binaryop->operandB);
		}
	}
}

void AECompiler::emitPushThis()
{
	emitInstruction(OP_PUSHTHIS);
}

void AECompiler::emitBinaryOp(aeNodeBinaryOperator* operation)
{
	if (m_logExprOps)
		emitDebugPrint("OP " + operation->oper + " " + operation->operandA->str() + " " + operation->operandB->str());

	if (operation->oper == "=")
	{
		emitAssignOp(operation->operandA, operation->operandB);
	}
	if (operation->oper == "+" || operation->oper == "-" || operation->oper == "*" || operation->oper == "/")
	{
		emitArithmeticOp(operation, aeExprContext());
	}
	if (operation->isRelational())
	{
		emitConditionalOp(operation/*, aeExprContext()*/);
	}
}

void AECompiler::emitArithmeticOp(aeNodeBinaryOperator* op, const aeExprContext& context)
{
	if (op->oper == "+")
	{
		aeExprContext ctxA = context;
		ctxA.rx_value = true;
		emitExpressionEval(op->operandA, ctxA);
		emitExpressionEval(op->operandB, ctxA);
		emitInstruction(OP_ADD, AEP_INT32);
	}

	if (op->oper == "-")
	{
		aeExprContext ctxA = context;
		ctxA.rx_value = true;
		emitExpressionEval(op->operandA, ctxA);
		emitExpressionEval(op->operandB, ctxA);
		emitInstruction(OP_SUB, AEP_INT32);
	}

	if (op->oper == "*")
	{
		aeExprContext ctxA = context;
		ctxA.rx_value = true;
		emitExpressionEval(op->operandA, ctxA);
		emitExpressionEval(op->operandB, ctxA);
		emitInstruction(OP_MUL, AEP_INT32);
	}

	if (op->oper == "/")
	{
		aeExprContext ctxA = context;
		ctxA.rx_value = true;
		emitExpressionEval(op->operandA, ctxA);
		emitExpressionEval(op->operandB, ctxA);
		emitInstruction(OP_DIV, AEP_INT32);
	}
}

bool AECompiler::canConvertType(AEType* typeA, AEType* typeB)
{
	return false;
}

void AECompiler::emitImplicitConversion(aeQualType typeA, aeQualType typeB)
{
	m_typeSystem.performConversion(typeA, typeB, this);
}

void AECompiler::emitLoadAddress(aeNodeExpr* expr)
{
	if (expr->m_nodeType != AEN_IDENTIFIER)
	{
		CompilerError("0002","emitLoadAddress: Only know how to load a variable ref");
		return;
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
//		emitInstruction(OP_LOAD, AEK_THIS, varStorage.type->getField(varStorage.name)->offset);
		//printf("'' beiNOT DONEng loaded from this->\n");
		emitPushThis();
		emitInstruction(OP_LOADADDR, AEK_THIS, varStorage.offset, 0);
	}
	else if (varStorage.mode == AE_VAR_GLOBAL)
	{
		CompilerLog("globals not working\n");
	}
	else
	{
		// The variable could not be found
		CompilerError("0002","The referenced variable cannot be found. '" + varExpr->m_name + "'");
		return;
	}
}

void AECompiler::emitLoadLiteral(aeNodeLiteral* lt)
{
	// Always leaves temporary rvalues in the stack

	if (lt->m_nodeType == AEN_INTEGER)
	{
		aeNodeInteger* integer_node = (aeNodeInteger*)lt;
		emitInstruction(OP_LOADK, AEK_INT, m_env->getIntegerLiteral(integer_node->value), 4);
	}
	else if (lt->m_nodeType == AEN_FLOAT)
	{
		aeNodeFloat* literal = (aeNodeFloat*)lt;
		emitInstruction(OP_LOADK, AEK_FLOAT, m_env->getFloatLiteral(literal->value), 4);
	}
	else if (lt->m_nodeType == AEN_STRING)
	{
		aeNodeString* literal = (aeNodeString*)lt;
		emitInstruction(OP_LOADK, AEK_STRING, m_env->getStringLiteral(literal->value), 4);
	}
}

void AECompiler::emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs)
{
	aeExprContext ectx;
	ectx.must_be_rvalue = true;

	aeQualType T1 = buildQualifiedType(lhs);
	aeQualType T2 = buildQualifiedType(rhs);
	
	if ((T1.getType() != T2.getType()) && !canImplicitlyConvert(T2, T1))
	{
		CompilerError("0002", "Cannot convert from " + T2.str() + " to " + T1.str());
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

void AECompiler::emitMemberOp(aeNodeAccessOperator* acs)
{
	/**
		We are compiling a.b
		b must be a part of a so it can be used
	*/

	//printf("Compiling operator.()\n");

	aeQualType Ta = buildQualifiedType(acs->m_a);
	if (!Ta)
	{
		CompilerError("0002","Cannot find the type of '" + acs->m_a->str() + "'");
		return;
	}

	aeExprContext exprContext;
	exprContext.expectedResult = Ta;
	if (Ta.m_handle)
		exprContext.rx_value = true; // for handles we want to load their value
	else
		exprContext.lvalue = true; // for values we want to get them loaded for edit
	emitExpressionEval(acs->m_a, exprContext);

	if (acs->m_b->m_nodeType == AEN_FUNCTIONCALL)
	{
		CompilerLog("Calling function on %s\n", Ta.str().c_str());

		// Emit the appropriate calling code, which assumes the arguments and obj to call on are pushed already
		emitFunctionCall(Ta, static_cast<aeNodeFunctionCall*>(acs->m_b), aeExprContext());
	}
	else
	{
		if (acs->m_b->m_nodeType == AEN_IDENTIFIER && Ta.getType()->isEnum())
		{
			aeNodeIdentifier* refNode = (aeNodeIdentifier*)acs->m_b;
			emitEnumValue((aeEnum*)Ta.getType(), refNode->m_name);
		}
	}
}

void AECompiler::emitConditionalOp(aeNodeBinaryOperator* operation)
{
	/*
		Operators: >, >=, <, <=, ==, !=, &&, ||

		These operators are handled together as they follow similar behavior.
		The result of the operation is put on the stack unless the expression
		context denies it.
		The result is either true or false, represented as bool.
	*/
	aeExprContext contextA;
	contextA.rx_value = true;
	emitExpressionEval(operation->operandA, contextA);

	aeExprContext contextB;
	contextB.rx_value = true;
	emitExpressionEval(operation->operandB, contextB);

	if (operation->oper == ">")
	{
		emitInstruction(OP_GT, 0);
	}

	if (operation->oper == "<")
	{
		emitDebugPrint("OP_LT " + operation->operandA->str() + " " + operation->operandB->str());
		emitInstruction(OP_LT, AEP_INT32);
	}

	if (operation->oper == ">=")
	{
		emitInstruction(OP_GTE, 0);
	}

	if (operation->oper == "<=")
	{
		emitInstruction(OP_LTE, 0);
	}

	if (operation->oper == "==")
	{
		emitInstruction(OP_EQ, 0);
	}
	
	if (operation->oper == "!=")
	{
		emitInstruction(OP_NEQ, 0);
	}
}

void AECompiler::emitVarExpr(aeNodeIdentifier* var, const aeExprContext& parentExprContext)
{
	/// The variable needs to be loaded into the stack, as it will be used to evaluate an expression
	auto varInfo = getVariable(var->m_name);

	int loadFrom = AEK_EBP;
	int offsetOnRefFrame = varInfo.offset;
	
	if (varInfo.mode == AE_VAR_FIELD)
	{
		// We're trying to access a member of "this"
		emitPushThis();
		loadFrom = AEK_THIS;
		emitDebugPrint("LOADING THIS." + var->m_name);
	}
	else if (varInfo.mode == AE_VAR_LOCAL)
	{
		if (varInfo.type.str() == "var"){
			emitInstruction(OP_PUSHVAR, varInfo.offset);
			printf(" Loading var: %s offset %d\n", varInfo.name.c_str(), varInfo.offset);
			return;
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
}

void AECompiler::emitNew(aeNodeNew* newExpr)
{
	// new X() expression must generate a new object instance.
	emitInstruction(OP_NEWOBJECT, 0);
}

void AECompiler::emitSubscriptOp(aeNodeSubscript* subscript)
{

}