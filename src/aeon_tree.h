#ifndef aeon_tree_h__
#define aeon_tree_h__

#include <string>
#include <vector>
#include <stdint.h>

class aeon_expression;
class ast_codeblock;

static std::string makeTabbing(int tabs)
{
	std::string buff; for (auto k = 0; k < tabs * 2; ++k) buff += ' ';
	return buff;
}

class aeon_ast_node
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

	std::vector<aeon_ast_node*> items;

public:

	void add(aeon_ast_node* n)
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
class ast_module : public aeon_ast_node
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
class ast_type : public aeon_ast_node
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

class ast_namespace : public aeon_ast_node
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

class ast_typedef : public aeon_ast_node
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


class ast_codeblock : public aeon_ast_node
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
class ast_enum : public aeon_ast_node
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
class ast_ifbranch : public aeon_ast_node
{
public:
	aeon_expression*      expr;
	ast_codeblock* block;
public:
	ast_ifbranch();
	std::string printtext();
};


class ast_return : public aeon_ast_node
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


class ast_class : public aeon_ast_node
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

/**
	\class aeon_ast_function
	\brief The root node of every function in the language

	These can be static class functions, methods,
	global functions.
*/
class aeon_ast_function : public aeon_ast_node
{
public:

	std::vector<aeon_expression*> m_parameters;
	std::string                   m_name;
	ast_type*                     m_return;
	ast_codeblock*                m_block;
	bool                          is_constructor;
	bool                          is_method;
	bool                          is_class_static;
	bool                          is_destructor;
	bool                          is_global;

public:
	aeon_ast_function();

	std::string printtext();
};

class ast_while : public aeon_ast_node
{
public:

	bool           doWhile;
	aeon_expression*      expr;
	ast_codeblock* block;

	ast_while();

	std::string printtext();
};

class ast_for : public aeon_ast_node
{
public:
	std::vector<aeon_expression*> initExpressions; ///< The for loop can have any number of init expressions separated by comma.
	aeon_expression*      expr;
	aeon_expression*      incrExpr;

	ast_codeblock* block;

	ast_for();

	std::string printtext();
};

class ast_using : public aeon_ast_node
{
public:

	aeon_expression* arg = nullptr;

	ast_using();

	std::string printtext();
};

#endif // aeon_tree_h__
