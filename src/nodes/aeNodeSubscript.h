#ifndef aeNodeSubscript_h__
#define aeNodeSubscript_h__

#include "aeNodeExpr.h"

/**
	\class aeNodeSubscriptOperator
	\brief The subscript operator in the form (subject)[argument]

	The subject can be a complex expression, evaluating to type T.
	The argument has to be compatible with any of the T::operator[] overloads.

	This operator will evaluate to the return type of the chosen T::operator[] overload.
*/
class aeNodeSubscript : public aeNodeExpr
{
public:
	aeNodeExpr* argument;
	aeNodeExpr* subject;

public:

	aeNodeSubscript();

	std::string exprstr();
};

#endif // aeNodeSubscript_h__
