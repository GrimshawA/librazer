#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include "aeon_vm.h"
#include "aeon_context.h"
#include "nodes/aeNodeNew.h"

#include <cassert>


void aeon_compiler::emitExpressionEval(aeNodeExpr* expr, aeExprContext exprContext)
{
	if (expr->m_nodeType == AEN_REF)
	{
		// Load the var address into the stack
		emitVarExpr(static_cast<aeNodeRef*>(expr), exprContext);
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

void aeon_compiler::emitFunctionCall(aeQualType beingCalledOn, aeNodeFunctionCall* fn, aeExprContext exprCtx)
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
				emitPushThis();
			}
		}
	}	

	aeFunction* func = m_env->getFunctionByName(finalSymbolName);
	if (!func)
	{
		throwError("Calling an unknown function " + finalSymbolName);
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

	
	if (func->m_native)
	{
		emitInstruction(OP_CALLMETHOD_NAT, m_env->getFunctionIndexByName(fn->m_name));
	}
	else
	{
		emitInstruction(OP_CALL, m_env->getFunctionIndexByName(finalSymbolName));
	}
}

void aeon_compiler::emitPushThis()
{
	emitInstruction(OP_PUSHTHIS);
}

void aeon_compiler::emitBinaryOp(aeNodeBinaryOperator* operation)
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
}

void aeon_compiler::emitArithmeticOp(aeNodeBinaryOperator* op, const aeExprContext& context)
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

bool aeon_compiler::canConvertType(aeType* typeA, aeType* typeB)
{
	return false;
}

void aeon_compiler::emitImplicitConversion(aeQualType typeA, aeQualType typeB)
{
	m_typeSystem.performConversion(typeA, typeB, this);
}

void aeon_compiler::emitLoadAddress(aeNodeExpr* expr)
{
	if (!expr->m_nodeType == AEN_REF)
	{
		throwError("emitLoadAddress: Only know how to load a variable ref");
		return;
	}
	aeNodeRef* varExpr = (aeNodeRef*)expr;

	auto varStorage = getVariable(varExpr->m_name);
	if (varStorage.mode == AE_VAR_LOCAL)
	{
		// Load a local variable address into the stack
		emitInstruction(OP_LOADADDR, AEK_EBP, varStorage.offset_bp);
		printf("Loading local\n");
	}
	else if (varStorage.mode == AE_VAR_FIELD)
	{
//		emitInstruction(OP_LOAD, AEK_THIS, varStorage.type->getField(varStorage.name)->offset);
		printf("'' beiNOT DONEng loaded from this->\n");
	}
	else if (varStorage.mode == AE_VAR_GLOBAL)
	{
		printf("globals not working\n");
	}
	else
	{
		// The variable could not be found
		throwError("The referenced variable cannot be found. '" + varExpr->m_name + "'");
		return;
	}
}

void aeon_compiler::emitLoadLiteral(aeNodeLiteral* lt)
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

void aeon_compiler::emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs)
{
	aeExprContext ectx;
	ectx.must_be_rvalue = true;

	aeType* lhs_type = evaluateType(lhs);
	aeType* rhs_type = evaluateType(rhs);
	/*if (!rhs_type)
	{
		throwError("emitAssignOp: Could not deduce the type the rhs expression evaluates to!");
		throwError("emitAssignOp: Here we would switch the compilation to runtime checking and dynamic typing!");
		return;
	}*/

	bool canDoAssignment = true;
	bool needsConversion = false;
	/*if (lhs_type != rhs_type)
	{
		if (canConvertType(lhs_type, rhs_type))
		{
			needsConversion = true;
			canDoAssignment = true;
		}
		else
		{
			throwError("Cannot convert from '' to ''");
			return;
		}

	}
	else
	{
		canDoAssignment = true;
	}*/

	if (canDoAssignment)
	{
		emitLoadAddress(lhs);

		// Send the right side of the assignment for evaluation
		aeExprContext exprContext;
		exprContext.expectedResult = buildQualifiedType(lhs);
		emitExpressionEval(rhs, exprContext);

		aeQualType qt2 = buildQualifiedType(rhs);

		throwError("Assigning from " + qt2.str() + " to " + exprContext.expectedResult.str());

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
}

void aeon_compiler::emitMemberOp(aeNodeAccessOperator* acs)
{
	/**
		We are compiling a.b
		b must be a part of a so it can be used
	*/

	//printf("Compiling operator.()\n");

	aeQualType Ta = buildQualifiedType(acs->m_a);
	if (!Ta)
	{
		throwError("Cannot find the type of '" + acs->m_a->str() + "'");
		return;
	}

	aeExprContext exprContext;
	exprContext.expectedResult = Ta;
	exprContext.rx_value = true;
	emitExpressionEval(acs->m_a, exprContext);

	if (acs->m_b->m_nodeType == AEN_FUNCTIONCALL)
	{
		// Emit the appropriate calling code, which assumes the arguments and obj to call on are pushed already
		emitFunctionCall(Ta, static_cast<aeNodeFunctionCall*>(acs->m_b), aeExprContext());
	}
	else
	{

	}
}

void aeon_compiler::emitConditionalOp(aeNodeBinaryOperator* operation)
{
	/*
		Operators: >, >=, <, <=, ==, !=, &&, ||

		These operators are handled together as they follow similar behavior.
		The result of the operation is put on the stack unless the expression
		context denies it.
		The result is either true or false, represented as bool.
	*/
	emitExpressionEval(operation->operandA, aeExprContext());
	emitExpressionEval(operation->operandB, aeExprContext());

	if (operation->oper == ">")
	{
		emitInstruction(OP_GT, 0);
	}

	if (operation->oper == "<")
	{
		emitInstruction(OP_LT, 0);
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

void aeon_compiler::emitVarExpr(aeNodeRef* var, const aeExprContext& parentExprContext)
{
	/// The variable needs to be loaded into the stack, as it will be used to evaluate an expression
	auto varInfo = getVariable(var->m_name);

	if (parentExprContext.rx_value)
	{
		int kind = AEP_INT32;

		if (parentExprContext.expectedResult.m_handle)
			kind = AEP_PTR;

		emitInstruction(OP_LOAD, AEK_EBP, varInfo.offset_bp, kind);
	}
	else
	{
		emitInstruction(OP_LOADADDR, AEK_EBP, varInfo.offset_bp);
	}
}

void aeon_compiler::emitNew(aeNodeNew* newExpr)
{
	// new X() expression must generate a new object instance.
	emitInstruction(OP_NEWOBJECT, 0);
}

void aeon_compiler::emitSubscriptOp(aeNodeSubscript* subscript)
{

}