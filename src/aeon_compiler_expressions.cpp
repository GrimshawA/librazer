#include "aeon_compiler.h"
#include "aeon_bytecode.h"
#include "aeon_vm.h"
#include "aeon_context.h"
#include "aeon_expression.h"

#include <cassert>


void aeon_compiler::emitExpressionEval(aeon_expression* expr, ExpressionEvalContext exprContext)
{
	if (expr->type == aeon_ast_node::VarExpr)
	{
		// push var contents to stack
		ast_varexpr* varexpr = static_cast<ast_varexpr*>(expr);

		aeon_instruction loadlocal;
		setopcode(loadlocal, EOpCodes::OP_LOAD);
		setinst_a(loadlocal, findLocalObject(varexpr->Name));
		emitInstruction(loadlocal);

		//Log("iload compiled");
	}
	else if (expr->type == aeon_ast_node::FuncCall)
	{
		emitFunctionCall(static_cast<ast_funccall*>(expr), ExpressionEvalContext());
	}
	else if (expr->type == aeon_ast_node::IntExpr || expr->type == aeon_ast_node::StringExpr || expr->type == aeon_ast_node::FloatExpr)
	{
		emitLoadLiteral((ast_literal*)expr);
	}
	else if (expr->type == aeon_ast_node::BinaryOperator)
	{
		ast_binaryop* binaryop = static_cast<ast_binaryop*>(expr);
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

void aeon_compiler::emitBinaryOp(ast_binaryop* operation)
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

void aeon_compiler::emitLoadAddress(aeon_expression* expr)
{
	if (!expr->type == aeon_expression::VarExpr)
	{
		throwError("emitLoadAddress: Only know how to load a variable ref");
		return;
	}
	ast_varexpr* varExpr = (ast_varexpr*)expr;

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

void aeon_compiler::emitLoadLiteral(ast_literal* lt)
{
	if (lt->type == aeon_ast_node::IntExpr)
	{
		ast_intexpr* integer_node = (ast_intexpr*)lt;
		emitInstruction(OP_LOADK, AEK_INT, m_env->getIntegerLiteral(integer_node->value));
	}
}

void aeon_compiler::emitAssignOp(aeon_expression* lhs, aeon_expression* rhs)
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

void aeon_compiler::emitConditionalOp(ast_binaryop* operation)
{
	/*
		Operators: >, >=, <, <=, ==, !=, &&, ||

		These operators are handled together as they follow similar behavior.
		The result of the operation is put on the stack unless the expression
		context denies it.
		The result is either true or false, represented as bool.
	*/

}

void aeon_compiler::emitVarExpr(ast_varexpr* var)
{
	/// The variable needs to be loaded into the stack, as it will be used to evaluate an expression
	
}