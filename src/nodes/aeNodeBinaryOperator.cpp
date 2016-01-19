#include "aeNodeBinaryOperator.h"

aeNodeBinaryOperator::aeNodeBinaryOperator(aeNodeExpr* opA, aeNodeExpr* opB, std::string _oper)
{
	m_type = BinaryOperator;

	operandA = opA;
	operandB = opB;
	oper = _oper;
}

std::string aeNodeBinaryOperator::exprstr()
{
	return std::string("(") + operandA->exprstr() + " " + oper + " " + operandB->exprstr() + ")";
}

std::string aeNodeBinaryOperator::printtext()
{
	return exprstr();
}