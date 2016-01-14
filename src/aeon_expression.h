#ifndef aeon_expression_h__
#define aeon_expression_h__

#include "aeon_tree.h"
#include <vector>
#include <stdint.h>


/**
	\class aeon_expression
	\brief Represents an arbitrary expression like an equation

	Supports infinite nesting of expressions and all kinds of different operations.
	Can evaluate its type and even results under certain constraints.
*/
class aeon_expression : public aeon_ast_node
{
public:

	aeon_expression* subscriptArgument = nullptr;

public:

	virtual std::string exprstr();

	bool isFloat();

	bool isInt();

	bool isString();

	int as_int();

	float as_float();

	std::string as_string();

	std::string printtext();
};

/// A function call can be standalone but is usually an operand
class ast_funccall : public aeon_expression
{
public:

	/// Arguments passed on to this function call
	std::vector<aeon_expression*> args;

	struct TemplateTypeArgument
	{
		std::string TypeString;
	};

	std::vector<TemplateTypeArgument> templateTypeArguments;

	ast_funccall()
	{
		type = FuncCall;
	}

	std::string funcName;

	virtual std::string exprstr()
	{
		std::string s1 = funcName;
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
		if (args.size() > 0)
		{
			for (std::size_t i = 0; i < args.size(); ++i)
			{
				s1 += args[i]->exprstr();
				if (i < args.size() - 1)
					s1 += ",";
			}
		}
		s1 += ")";

		if (subscriptArgument)
		{
			s1 += "[" + subscriptArgument->exprstr() + "]";
		}

		if (items.size() > 0)
		{
			s1 += "." + static_cast<aeon_expression*>(items[0])->exprstr();
		}
		return s1;
	}

	std::string printtext()
	{
		std::string s1 = std::string("Call ") + exprstr();
		return s1;
	}
};

class ast_stringexpr : public aeon_expression
{
public:
	ast_stringexpr()
	{
		type = StringExpr;
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


class ast_floatexpr : public aeon_expression
{
public:
	ast_floatexpr()
	{
		type = FloatExpr;
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

class ast_intexpr : public aeon_expression
{
public:
	ast_intexpr()
	{
		type = IntExpr;
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

class ast_varexpr : public aeon_expression
{
public:

	ast_varexpr()
	{
		type = VarExpr;
	}

	bool explicitDeclaration;

	std::string TypeString;
	ast_type* VarType;
	std::string Name;

	virtual std::string exprstr()
	{
		std::string s1 = Name;
		if (subscriptArgument)
		{
			s1 += "[" + subscriptArgument->exprstr() + "]";
		}

		if (items.size() > 0)
		{
			s1 += "." + static_cast<aeon_expression*>(items[0])->exprstr();
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
class ast_binaryop : public aeon_expression
{
public:

	ast_binaryop(aeon_expression* opA, aeon_expression* opB, std::string _oper)
	{
		type = BinaryOperator;

		operandA = opA;
		operandB = opB;
		oper = _oper;
	}

	virtual std::string exprstr()
	{
		return std::string("(") + operandA->exprstr() + " " + oper + " " + operandB->exprstr() + ")";
	}

	aeon_expression* operandA;
	aeon_expression* operandB;

	std::string oper;

	std::string printtext()
	{
		return exprstr();
	}
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
class ast_unaryop : public aeon_expression
{
public:

	std::string OperatorString;
	aeon_expression* Operand;

	ast_unaryop()
	{
		type = UnaryOperator;

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
class aeon_expression_subscript : public aeon_expression
{
public:
	aeon_expression* argument;
	aeon_expression* subject;

	std::string exprstr()
	{
		return "(" + subject->exprstr() + ")[" + argument->exprstr() + "]";
	}
};

#endif // aeon_expression_h__
