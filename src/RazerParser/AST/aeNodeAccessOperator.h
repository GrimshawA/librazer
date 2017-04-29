#ifndef aeNodeAccessOperator_h__
#define aeNodeAccessOperator_h__

#include "aeNodeExpr.h"

/**
	\class aeNodeAccessOperator
	\brief Represents an access to a member of some construct

	It doesn't necessarily have to the be the dot (.) operator,
	but its the one implemented now.

	It represents an expression:

	expr := a.b

	Evaluates to the type of the b expr.
	In the absence of either sub expr, an error happens.
*/
class aeNodeAccessOperator : public aeNodeExpr
{
public:

	enum AccessOperatorType
	{
		Dot,
		DerefAccess
	};

	aeNodeExpr*   m_a;
	aeNodeExpr*   m_b;
	AccessOperatorType            m_accessType;

public:

	aeNodeAccessOperator();

	std::string str() const;
};

#endif // aeNodeAccessOperator_h__
