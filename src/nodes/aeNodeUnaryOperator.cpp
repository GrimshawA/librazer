#include "aeNodeUnaryOperator.h"

aeNodeUnaryOperator::aeNodeUnaryOperator()
{
	m_nodeType = UnaryOperator;

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