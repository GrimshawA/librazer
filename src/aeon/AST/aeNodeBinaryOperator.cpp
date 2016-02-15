#include "aeNodeBinaryOperator.h"
#include <AEON/Compiler/aeCompiler.h>
#include <AEON/aeContext.h>

aeNodeBinaryOperator::aeNodeBinaryOperator(aeNodeExpr* opA, aeNodeExpr* opB, std::string _oper)
{
	m_nodeType = AEN_BINARYOP;

	operandA = opA;
	operandB = opB;
	oper = _oper;
}

bool aeNodeBinaryOperator::isRelational()
{
	return (oper == ">") || (oper == ">=") || (oper == "<") || (oper == "<=")
		|| (oper == "==") || (oper == "!=");
}

bool aeNodeBinaryOperator::isArithmetic()
{
	return oper == "+" || oper == "-" || oper == "*" || oper == "/";
}

aeQualType aeNodeBinaryOperator::getQualifiedType(aeCompiler* c)
{
	aeQualType qt;
	if (isRelational())
	{
		qt.m_type = c->m_env->getTypeInfo("bool");
	}
	else if (isArithmetic())
	{
		qt.m_type = c->m_env->getTypeInfo("int32");
	}

	return qt;
}

std::string aeNodeBinaryOperator::str() const
{
	return std::string("(") + operandA->str() + " " + oper + " " + operandB->str() + ")";
}