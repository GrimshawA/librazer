#include "aeNodeBinaryOperator.h"
#include <RazerCompiler/RzCompiler.h>
#include <RazerRuntime/RzEngine.h>
#include <Logger.h>

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
        || (oper == "==") || (oper == "!=") || (oper == "||") || (oper == "&&");
}

bool aeNodeBinaryOperator::isArithmetic()
{
	return oper == "+" || oper == "-" || oper == "*" || oper == "/";
}

std::string aeNodeBinaryOperator::str() const
{
	if (!operandA || !operandB)
		return std::string();

	return std::string("(") + operandA->str() + " " + oper + " " + operandB->str() + ")";
}
