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

/// A function call can be standalone but is usually an operand
class aeNodeFunctionCall : public aeNodeExpr
{
public:

	std::vector<aeNodeExpr*> m_args;        ///< List of arguments to this function call
	aeNodeFunction*          m_callee;      ///< The node we are calling to execute
	bool                     m_is_method;   ///< This node calls a method on an object or static class function
	std::string              m_name;


	struct TemplateTypeArgument
	{
		std::string TypeString;
	};

	std::vector<TemplateTypeArgument> templateTypeArguments;

public:

	aeNodeFunctionCall()
	{
		m_type = FuncCall;
	}


	virtual std::string exprstr()
	{
		std::string s1 = m_name;
		if (templateTypeArguments.size() > 0)
		{
			s1 += "<";
			for (std::size_t i = 0; i < templateTypeArguments.size(); ++i)
			{
				s1 += templateTypeArguments[i].TypeString;
				if (i < templateTypeArguments.size() - 1)
					s1 += ",";
			}
			s1 += ">";
		}
		s1 += "(";
		if (m_args.size() > 0)
		{
			for (std::size_t i = 0; i < m_args.size(); ++i)
			{
				s1 += m_args[i]->exprstr();
				if (i < m_args.size() - 1)
					s1 += ",";
			}
		}
		s1 += ")";

		if (m_items.size() > 0)
		{
			s1 += "." + static_cast<aeNodeExpr*>(m_items[0])->exprstr();
		}
		return s1;
	}

	std::string printtext()
	{
		std::string s1 = std::string("Call ") + exprstr();
		return s1;
	}
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


// A binary op can be any expression A and B with an operator in the middle, like 5 + 10, B * C, myCall() + 5, etc
// operands can be literals, vars and other operations
class aeNodeBinaryOperator : public aeNodeExpr
{
public:

	aeNodeBinaryOperator(aeNodeExpr* opA, aeNodeExpr* opB, std::string _oper)
	{
		m_type = BinaryOperator;

		operandA = opA;
		operandB = opB;
		oper = _oper;
	}

	virtual std::string exprstr()
	{
		return std::string("(") + operandA->exprstr() + " " + oper + " " + operandB->exprstr() + ")";
	}

	aeNodeExpr* operandA;
	aeNodeExpr* operandB;

	std::string oper;

	std::string printtext()
	{
		return exprstr();
	}
};

class aeNodeTernaryOperator : public aeNodeExpr
{
public:
	aeNodeExpr* lhs;
	aeNodeExpr* middle;
	aeNodeExpr* rhs;
};

/**
\class ast_unaryop

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

	aeNodeUnaryOperator()
	{
		m_type = UnaryOperator;

		Operand = nullptr;
	}

	virtual std::string exprstr()
	{
		return std::string(OperatorString + Operand->exprstr());
	}

	std::string printtext()
	{
		return exprstr();
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
