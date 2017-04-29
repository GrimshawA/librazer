#ifndef aeNodeUnaryOperator_h__
#define aeNodeUnaryOperator_h__

#include "aeNodeExpr.h"

/**
	\class aeNodeUnaryOperator

	List unary operations:
	-(var)
	++var
	--var
	var++
	var--
	*var (deref)
	typeof(operand)
*/
class aeNodeUnaryOperator : public aeNodeExpr
{
public:

	std::string OperatorString;
	aeNodeExpr* Operand;

public:
	aeNodeUnaryOperator();

	virtual std::string exprstr();

	std::string str();
};


#endif // aeNodeUnaryOperator_h__
