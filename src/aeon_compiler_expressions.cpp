#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include "aeon_vm.h"
#include "aeon_context.h"
#include "aeon_expression.h"

#include <cassert>


void aeon_compiler::emitExpressionEval(aeNodeExpr* expr, ExpressionEvalContext exprContext)
{
	if (expr->m_nodeType == VarExpr)
	{
		// Load the var address into the stack
		emitVarExpr(static_cast<aeNodeVarRef*>(expr));
	}
	else if (expr->m_nodeType == AEN_FUNCTIONCALL)
	{
		emitFunctionCall(static_cast<aeNodeFunctionCall*>(expr), ExpressionEvalContext());
	}
	else if (expr->m_nodeType == AEN_ACCESSOPERATOR)
	{
		emitAccessOp(static_cast<aeNodeAccessOperator*>(expr));
	}
	else if (expr->m_nodeType == IntExpr || expr->m_nodeType == StringExpr || expr->m_nodeType == FloatExpr)
	{
		emitLoadLiteral((aeNodeLiteral*)expr);
	}
	else if (expr->m_nodeType == BinaryOperator)
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

void aeon_compiler::emitFunctionCall(aeNodeFunctionCall* fnCall, ExpressionEvalContext exprCtx)
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
	if (!expr->m_nodeType == VarExpr)
	{
		throwError("emitLoadAddress: Only know how to load a variable ref");
		return;
	}
	aeNodeVarRef* varExpr = (aeNodeVarRef*)expr;

	auto varStorage = getVariable(varExpr->Name);
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
		throwError("The referenced variable cannot be found. '" + varExpr->Name + "'");
		return;
	}
}

void aeon_compiler::emitLoadLiteral(aeNodeLiteral* lt)
{
	if (lt->m_nodeType == IntExpr)
	{
		aeNodeInteger* integer_node = (aeNodeInteger*)lt;
		emitInstruction(OP_LOADK, AEK_INT, m_env->getIntegerLiteral(integer_node->value));
	}
}

void aeon_compiler::emitAssignOp(aeNodeExpr* lhs, aeNodeExpr* rhs)
{
	ExpressionEvalContext ectx;
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
		emitExpressionEval(rhs, ectx);
		if (needsConversion)
			emitConversion(lhs_type, rhs_type);
		emitInstruction(OP_SET, 0, 0, 1);
	}
}

void aeon_compiler::emitAccessOp(aeNodeAccessOperator* acs)
{
	printf("Compiling operator.()\n");

	/*aeQualType Atype = getExpressionQualType(acs->m_a);
	aeQualType Btype = getExpressionQualType(acs->m_a);*/

	emitExpressionEval(acs->m_a, ExpressionEvalContext());

	if (acs->m_b->m_nodeType == AEN_FUNCTIONCALL)
	{
		// Emit the appropriate calling code, which assumes the arguments and obj to call on are pushed already
		emitFunctionCall(static_cast<aeNodeFunctionCall*>(acs->m_b), ExpressionEvalContext());
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

}

void aeon_compiler::emitVarExpr(aeNodeVarRef* var)
{
	/// The variable needs to be loaded into the stack, as it will be used to evaluate an expression
	auto& varInfo = getVariable(var->Name);
	emitInstruction(OP_LOAD, AEK_EBP, varInfo.offset_bp);
}