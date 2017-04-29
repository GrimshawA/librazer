#ifndef aeNodeBinaryOperator_h__
#define aeNodeBinaryOperator_h__

#include "aeNodeExpr.h"

/**
	\class aeNodeBinaryOperator

	A binary op can be any expression A and B with an operator in the middle, like 5 + 10, B * C, myCall() + 5, etc
	operands can be literals, vars and other operations
*/
class aeNodeBinaryOperator : public aeNodeExpr
{
public:

	aeNodeExpr* operandA;
	aeNodeExpr* operandB;

	std::string oper;

public:

	aeNodeBinaryOperator(aeNodeExpr* opA, aeNodeExpr* opB, std::string _oper);

	bool isRelational();
	bool isArithmetic();

	std::string str() const;
};

#endif // aeNodeBinaryOperator_h__
