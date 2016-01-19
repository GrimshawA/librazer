#ifndef aeon_expression_h__
#define aeon_expression_h__

#include "nodes/aeNodeExpr.h"

#include "aeon_tree.h"
#include <vector>
#include <stdint.h>

class aeNodeThis : public aeNodeExpr
{
public:
	bool        m_is_implicit;   ///< Was 'this' actually written or inferred from context 
	aeNodeExpr* m_expr;          ///< The expression after the this
};


class aeNodeLiteral : public aeNodeExpr
{
public:

};

class aeNodeString : public aeNodeLiteral
{
public:
	aeNodeString()
	{
		m_type = StringExpr;
	}

	virtual std::string exprstr()
	{
		return "\"" + value + "\"";
	}

	std::string printtext()
	{
		return std::string(" string '") + value + "'";
	}

	std::string value;
};


class aeNodeFloat : public aeNodeLiteral
{
public:
	aeNodeFloat()
	{
		m_type = FloatExpr;
	}

	virtual std::string exprstr()
	{
		return std::to_string(value);
	}

	std::string printtext()
	{
		return std::string("Float - ") + std::to_string(value);
	}

	float value;
};

class aeNodeInteger : public aeNodeLiteral
{
public:
	aeNodeInteger()
	{
		m_type = IntExpr;
	}

	virtual std::string exprstr()
	{
		return std::to_string(value);
	}

	std::string printtext()
	{
		return std::string("Int - ") + std::to_string(value);
	}

	uint32_t value;
};

class aeNodeVarRef : public aeNodeExpr
{
public:

	aeNodeVarRef()
	{
		m_type = VarExpr;
	}

	bool explicitDeclaration;

	std::string TypeString;
	aeNodeTypeDecl* VarType;
	std::string Name;

	virtual std::string exprstr()
	{
		std::string s1 = Name;

		if (m_items.size() > 0)
		{
			s1 += "." + static_cast<aeNodeExpr*>(m_items[0])->exprstr();
		}
		return s1;
	}

	std::string printtext()
	{
		return std::string("VarExpr - ") + Name;
	}
};

/**
	\class aeon_expression_subscript
	\brief The subscript operator in the form (subject)[argument]

	The subject can be a complex expression, evaluating to type T.
	The argument has to be compatible with any of the T::operator[] overloads.

	This operator will evaluate to the return type of the chosen T::operator[] overload.
*/
class aeNodeSubscriptOperator : public aeNodeExpr
{
public:
	aeNodeExpr* argument;
	aeNodeExpr* subject;

	std::string exprstr()
	{
		return "(" + subject->exprstr() + ")[" + argument->exprstr() + "]";
	}
};

#endif // aeon_expression_h__
