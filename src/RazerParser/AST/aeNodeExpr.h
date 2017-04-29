#ifndef aeNodeExpr_h__
#define aeNodeExpr_h__

#include <RazerParser/AST/AEBaseNode.h>
#include "aeNodeValue.h"

#include <map>
#include <memory>

class RzCompiler;

/**
	\class aeNodeExpr
	\brief Represents an arbitrary expression like an equation

	Supports infinite nesting of expressions and all kinds of different operations.
	Can evaluate its type and even results under certain constraints.
*/
class aeNodeExpr : public aeNodeValue
{
public:

	aeNodeExpr*              m_parentExpr;  ///< The parent expression who called this function (or nullptr for top level stmt)

public:

	aeNodeExpr();

	/// Get the parent expression if any
	aeNodeExpr* getParentExpression();

	bool isFloat();

	bool isInt();

	bool isString();

	int as_int();

	float as_float();

	std::string as_string();

	std::string str() const;
};

#endif // aeNodeExpr_h__
