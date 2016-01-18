#ifndef aeNodeExpr_h__
#define aeNodeExpr_h__

#include "aeNodeStatement.h"

#include <map>
#include <memory>

/**
	\class aeNodeExpr
	\brief Represents an arbitrary expression like an equation

	Supports infinite nesting of expressions and all kinds of different operations.
	Can evaluate its type and even results under certain constraints.
*/
class aeNodeExpr : public aeNodeStatement
{
public:

	aeNodeExpr*              m_parentExpr;  ///< The parent expression who called this function (or nullptr for top level stmt)

public:

	aeNodeExpr();

	virtual std::string exprstr();

	bool isFloat();

	bool isInt();

	bool isString();

	int as_int();

	float as_float();

	std::string as_string();

	std::string printtext();
};

#endif // aeNodeExpr_h__
