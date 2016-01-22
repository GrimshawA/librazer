#include "aeNodeUnaryOperator.h"

aeNodeUnaryOperator::aeNodeUnaryOperator()
{
	m_nodeType = AEN_UNARYOP;

	Operand = nullptr;
}

std::string aeNodeUnaryOperator::exprstr()
{
	return std::string(OperatorString + Operand->exprstr());
}

std::string aeNodeUnaryOperator::printtext()
{
	return exprstr();
}