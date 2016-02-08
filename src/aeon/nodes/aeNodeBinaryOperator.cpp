#include "aeNodeBinaryOperator.h"
#include "../aeon_compiler.h"
#include "../aeon_context.h"

aeNodeBinaryOperator::aeNodeBinaryOperator(aeNodeExpr* opA, aeNodeExpr* opB, std::string _oper)
{
	m_nodeType = AEN_BINARYOP;

	operandA = opA;
	operandB = opB;
	oper = _oper;
}

bool aeNodeBinaryOperator::isBooleanOperator()
{
	return (oper == ">") || (oper == ">=") || (oper == "<") || (oper == "<=")
		|| (oper == "==") || (oper == "!=");
}

bool aeNodeBinaryOperator::isArithmetic()
{
	return oper == "+" || oper == "-" || oper == "*" || oper == "/";
}

aeQualType aeNodeBinaryOperator::getQualifiedType(aeon_compiler* c)
{
	aeQualType qt;
	if (isBooleanOperator())
	{
		qt.m_type = c->m_env->getTypeInfo("bool");
	}
	else if (oper == "+")
	{
		qt.m_type = c->m_env->getTypeInfo("int32");
	}

	return qt;
}

std::string aeNodeBinaryOperator::str() const
{
	return std::string("(") + operandA->str() + " " + oper + " " + operandB->str() + ")";
}