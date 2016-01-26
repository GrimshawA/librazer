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
		emitVarExpr(static_cast<aeNodeRef*>(expr));
	}
	else if (expr->m_nodeType == AEN_FUNCTIONCALL)
	{
		emitFunctionCall(static_cast<aeNodeFunctionCall*>(expr), aeExprContext());
	}
	else if (expr->m_nodeType == AEN_ACCESSOPERATOR)
	{
		emitAccessOp(static_cast<aeNodeAccessOperator*>(expr));
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
		else if (binaryop->oper == "+" || binaryop->oper == "-")
		{
			emitBinaryOp(binaryop);
		}
		else if (binaryop->oper == "=")
		{
			emitAssignOp(binaryop->operandA, binaryop->operandB);
		}
	}
}

void aeon_compiler::emitFunctionCall(aeNodeFunctionCall* fnCall, aeExprContext exprCtx)
{

	int funcId = m_env->getNativeBehaviorIndex("GameObject", fnCall->m_name);
	if (funcId >= 0)
	{
		emitDebugPrint("Calling setHEalth");
		emitInstruction(OP_CALLMETHOD_NAT, funcId);
	}
	else
	{
		if (m_caller->isNonStaticMethod())
		{
			// If we are calling this from a method, priority is to call
			aeon_type* classTypeInfo = evaluateType(m_classes.back());
			for (auto& method : classTypeInfo->m_methods)
			{
				if (method.name == fnCall->m_name)
				{
					//emitInstruction(OP_CALL, m_env->getFunctionIndexByName());
					return;
				}
			}
		}
		else
		{
			//emitInstruction(OP_CALL, m_env->getFunctionByName(m_caller)
		}

		// Aeon call
		if (m_module->getFunctionIndexByName(fnCall->m_name) < 0)
		{
			throwError("Function '" + fnCall->m_name + "' not found!");
		}
		else
		{
			// we'll need to call a nested func
			aeon_instruction fcallinst;
			setopcode(fcallinst, EOpCodes::OP_CALL);
			setinst_a(fcallinst, m_module->getFunctionIndexByName(fnCall->m_name));
			setinst_b(fcallinst, fnCall->m_items.size());
			emitInstruction(fcallinst);
		}
	}
}

void aeon_compiler::emitBinaryOp(aeNodeBinaryOperator* operation)
{
	if (operation->oper == "=")
	{
		emitAssignOp(operation->operandA, operation->operandB);
	}
}

bool aeon_compiler::canConvertType(aeon_type* typeA, aeon_type* typeB)
{
	return false;
}

void aeon_compiler::emitConversion(aeon_type* typeA, aeon_type* typeB)
{

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
		emitInstruction(OP_LOAD, AEK_EBP, varStorage.offset_bp);
		printf("Loading local\n");
	}
	else if (varStorage.mode == AE_VAR_FIELD)
	{
		emitInstruction(OP_LOAD, AEK_THIS, varStorage.type->getField(varStorage.name)->offset);
		printf("'' being loaded from this->\n");
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
	if (lt->m_nodeType == AEN_INTEGER)
	{
		aeNodeInteger* integer_node = (aeNodeInteger*)lt;
		emitInstruction(OP_LOADK, AEK_INT, m_env->getIntegerLiteral(integer_node->value));
	}
}

void aeon_compiler::emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs)
{
	aeExprContext ectx;
	ectx.must_be_rvalue = true;

	aeon_type* lhs_type = evaluateType(lhs);
	aeon_type* rhs_type = evaluateType(rhs);
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

		// Finalize the assignment
		emitInstruction(OP_SET, 0, 0, 1);
	}
}

void aeon_compiler::emitAccessOp(aeNodeAccessOperator* acs)
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

	emitExpressionEval(acs->m_a, aeExprContext());

	if (acs->m_b->m_nodeType == AEN_FUNCTIONCALL)
	{
		// Emit the appropriate calling code, which assumes the arguments and obj to call on are pushed already
		emitFunctionCall(static_cast<aeNodeFunctionCall*>(acs->m_b), aeExprContext());
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

void aeon_compiler::emitVarExpr(aeNodeRef* var)
{
	/// The variable needs to be loaded into the stack, as it will be used to evaluate an expression
	auto& varInfo = getVariable(var->m_name);
	emitInstruction(OP_LOAD, AEK_EBP, varInfo.offset_bp);
}

void aeon_compiler::emitNew(aeNodeNew* newExpr)
{
	// new X() expression must generate a new object instance.
	emitInstruction(OP_NEWOBJECT, 0);
}

void aeon_compiler::emitSubscriptOp(aeNodeSubscript* subscript)
{

}