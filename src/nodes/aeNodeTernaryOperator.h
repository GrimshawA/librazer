#ifndef aeNodeTernaryOperator_h__
#define aeNodeTernaryOperator_h__

#include "aeNodeExpr.h"

/**
	\class aeNodeTernaryOperator
*/
class aeNodeTernaryOperator : public aeNodeExpr
{
public:
	aeNodeExpr* lhs;
	aeNodeExpr* middle;
	aeNodeExpr* rhs;

public:
	aeNodeTernaryOperator();
};

#endif // aeNodeTernaryOperator_h__
