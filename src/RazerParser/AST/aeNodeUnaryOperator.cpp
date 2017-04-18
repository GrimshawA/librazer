#include "aeNodeUnaryOperator.h"

aeNodeUnaryOperator::aeNodeUnaryOperator()
{
	m_nodeType = AEN_UNARYOP;

	Operand = nullptr;
}

std::string aeNodeUnaryOperator::exprstr()
{
	return std::string(OperatorString + Operand->str());
}

RzQualType aeNodeUnaryOperator::getQualifiedType(RzCompiler* c) {
    return Operand->getQualifiedType(c);
}

std::string aeNodeUnaryOperator::str()
{
	return exprstr();
}
