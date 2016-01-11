#ifndef aeon_tree_h__
#define aeon_tree_h__

#include <string>
#include <vector>
#include <stdint.h>

class ast_expr;
class ast_codeblock;

static std::string makeTabbing(int tabs)
{
	std::string buff; for (auto k = 0; k < tabs * 2; ++k) buff += ' ';
	return buff;
}

class atom_ast_node
{
public:

	enum NodeTypes
	{
		Module,
		Namespace,
		Using,
		ForwardClassDecl,
		ClassDecl,
		VarDecl,
		ForwardFuncDecl,
		FuncDecl,
		UsingNamespace,
		StackScope,
		NamespaceDecl,
		PragmaDecl,
		VisibilityMutation,
		CharLiteral,
		StringExpr,
		IntExpr,
		FloatExpr,
		BinaryOperator,
		UnaryOperator,
		FuncCall,
		IfBranch,
		CodeBlock,
		WhileLoop,
		ForLoop,
		Return,
		VarExpr,
		Enum,
		Type,
		Typedef,
	};

	int type;

	std::vector<atom_ast_node*> items;

public:

	void add(atom_ast_node* n)
	{
		items.push_back(n);
	}

	virtual std::string printtext()
	{
		return ".";
	}

	virtual void printSelf(int tabs = 0)
	{
		printf("%s%s\n", makeTabbing(tabs).c_str(), printtext().c_str());
		for (auto item : items)
		{
			item->printSelf(tabs + 1);
		}
	}
};

// this is the root of every translation unit
class ast_module : public atom_ast_node
{
public:
	ast_module()
	{
		type = Module;
	}

	std::string ModuleName;

	virtual void printSelf(int tabs)
	{
		printf("%sModule '%s'\n", makeTabbing(tabs).c_str(), ModuleName.c_str());
		for (auto item : items)
		{
			item->printSelf(tabs + 1);
		}
	}
};

/// The if in code, spawns an ifbranch with two children, the expr and the block of code
class ast_type : public atom_ast_node
{
public:

	std::string name;

	std::vector<ast_type*> templateTypeArguments;

	ast_type()
	{
		type = Type;
	}

	std::string printtext()
	{
		return "Type " + name;
	}
};

class ast_namespace : public atom_ast_node
{
public:

	ast_namespace()
	{
		type = Namespace;
	}

	std::string Name;

	std::string printtext()
	{
		return "Namespace";
	}
};

class ast_typedef : public atom_ast_node
{
public:

	ast_typedef()
	{
		type = Typedef;
	}

	ast_type* typeA;
	ast_type* typeB;

	std::string printtext()
	{
		return "Typedef";
	}
};


class ast_codeblock : public atom_ast_node
{
public:

	ast_codeblock()
	{
		type = CodeBlock;
	}

	virtual void printSelf(int tabs)
	{
		printf("%sBlock '%s'\n", makeTabbing(tabs).c_str(), "{}");
		for (auto item : items)
		{
			item->printSelf(tabs + 1);
		}
	}
};



/// The if in code, spawns an ifbranch with two children, the expr and the block of code
class ast_enum : public atom_ast_node
{
public:

	std::string name;
	std::vector<std::string> members;

	ast_enum()
	{
		type = Enum;
	}

	void addField(std::string name)
	{
		members.push_back(name);
	}

	std::string printtext()
	{
		return "Enum " + name;
	}
};



/// The if in code, spawns an ifbranch with two children, the expr and the block of code

/// An expression is something that evaluates to something meaningful, a value, like what can be assigned to something, or what can be passed to a func
class ast_expr : public atom_ast_node
{
public:

	ast_expr* subscriptArgument = nullptr;

	virtual std::string exprstr()
	{
		return "";
	}

	bool isFloat()
	{
		return type == FloatExpr;
	}

	bool isInt()
	{
		return type == IntExpr;
	}

	bool isString()
	{
		return type == StringExpr;
	}

	int as_int();

	float as_float();

	std::string as_string();

	std::string printtext()
	{
		return "Expr";
	}
};

/// The if in code, spawns an ifbranch with two children, the expr and the block of code
class ast_ifbranch : public atom_ast_node
{
public:

	ast_expr*      expr;
	ast_codeblock* block;

	ast_ifbranch()
	{
		type = IfBranch;

		// we have to have an expression and block so..
		block = new ast_codeblock();
		add(block);
	}

	std::string printtext()
	{
		return "if " + expr->exprstr();
	}
};

/// A function call can be standalone but is usually an operand
class ast_funccall : public ast_expr
{
public:

	/// Arguments passed on to this function call
	std::vector<ast_expr*> args;

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
			s1 += "." + static_cast<ast_expr*>(items[0])->exprstr();
		}
		return s1;
	}

	std::string printtext()
	{
		std::string s1 = std::string("Call ") + exprstr();
		return s1;
	}
};

class ast_stringexpr : public ast_expr
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


class ast_floatexpr : public ast_expr
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

class ast_intexpr : public ast_expr
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

class ast_return : public atom_ast_node
{
public:
	ast_return()
	{
		type = Return;
	}

	std::string printtext()
	{
		return std::string("Return");
	}
};


class ast_class : public atom_ast_node
{
public:

	std::string Name;

	struct classparentinfo
	{
		std::string parentClass;
		std::string accessLevel;
	};

	std::vector<classparentinfo> parents;

	ast_class()
	{
		type = ClassDecl;
	}

	virtual void printSelf(int tabs)
	{
		printf("%sClassDecl '%s' %d parents\n", makeTabbing(tabs).c_str(), Name.c_str(), parents.size());
		for (auto item : items)
		{
			item->printSelf(tabs + 1);
		}
	}
};

class ast_varexpr : public ast_expr
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
			s1 += "." + static_cast<ast_expr*>(items[0])->exprstr();
		}
		return s1;
	}

	std::string printtext()
	{
		return std::string("VarExpr - ") + Name;
	}
};

class ast_func : public atom_ast_node
{
public:

	std::vector<ast_expr*> parameters;
	ast_codeblock* block;

	ast_func()
	{
		type = FuncDecl;

		block = new ast_codeblock();
		add(block);
	}

	std::string TypeString;
	std::string Name;

	std::string printtext()
	{
		std::string s1 = std::string("Fn ") + Name + "(";
		if (parameters.size() > 0)
		{
			for (std::size_t i = 0; i < parameters.size(); ++i)
			{
				s1 += static_cast <ast_varexpr*>(parameters[i])->TypeString;
				if (i < parameters.size() - 1)
					s1 += ",";
			}
		}
		s1 += ")";
		return s1;
	}
};

class ast_while : public atom_ast_node
{
public:

	bool           doWhile;
	ast_expr*      expr;
	ast_codeblock* block;

	ast_while()
	{
		type = WhileLoop;

		doWhile = false;

		block = new ast_codeblock();
		add(block);
	}

	std::string printtext()
	{
		return std::string("While");
	}
};

class ast_for : public atom_ast_node
{
public:
	ast_expr*      initExpr;
	ast_expr*      expr;
	ast_expr*      incrExpr;
	ast_codeblock* block;

	ast_for()
	{
		type = ForLoop;

		block = new ast_codeblock();
		add(block);
	}

	std::string printtext()
	{
		return std::string("For");
	}
};

// A binary op can be any expression A and B with an operator in the middle, like 5 + 10, B * C, myCall() + 5, etc
// operands can be literals, vars and other operations
class ast_binaryop : public ast_expr
{
public:

	ast_binaryop(ast_expr* opA, ast_expr* opB, std::string _oper)
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

	ast_expr* operandA;
	ast_expr* operandB;

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
class ast_unaryop : public ast_expr
{
public:

	std::string OperatorString;
	ast_expr* Operand;

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

class ast_using : public atom_ast_node
{
public:

	ast_expr* arg = nullptr;

	ast_using()
	{
		type = Using;
	}

	std::string printtext()
	{
		return std::string("Using ") + arg->exprstr();
	}
};

#endif // aeon_tree_h__
