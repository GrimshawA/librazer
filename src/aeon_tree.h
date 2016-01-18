#ifndef aeon_tree_h__
#define aeon_tree_h__

#include "nodes/aeNodeBase.h"
#include "nodes/aeNodeModule.h"
#include "nodes/aeNodeStatement.h"

#include <string>
#include <vector>
#include <stdint.h>

class aeNodeExpr;
class aeNodeBlock;

static std::string makeTabbing(int tabs)
{
	std::string buff; for (auto k = 0; k < tabs * 2; ++k) buff += ' ';
	return buff;
}


/**
	\class aeNodeEcosystem

	The ecosystem harbors an arbitrary number of modules, it is used to build a complete tree with all modules in the software.
	Allows traversing and other kinds of instrospection.
*/
class aeNodeEcosystem : public aeNodeBase
{
public:
	std::vector<aeNodeModule> modules;
};

/// The if in code, spawns an ifbranch with two children, the expr and the block of code
class aeNodeTypeDecl : public aeNodeBase
{
public:

	std::string name;

	std::vector<aeNodeTypeDecl*> templateTypeArguments;

	aeNodeTypeDecl()
	{
		m_type = Type;
	}

	std::string printtext()
	{
		return "Type " + name;
	}
};

class aeNodeTypedef : public aeNodeBase
{
public:

	aeNodeTypedef()
	{
		m_type = Typedef;
	}

	aeNodeTypeDecl* typeA;
	aeNodeTypeDecl* typeB;

	std::string printtext()
	{
		return "Typedef";
	}
};


class aeNodeBlock : public aeNodeStatement
{
public:

	aeNodeBlock()
	{
		m_type = CodeBlock;
	}

	virtual void printSelf(int tabs)
	{
		printf("%sBlock '%s'\n", makeTabbing(tabs).c_str(), "{}");
		for (auto item : m_items)
		{
			item->printSelf(tabs + 1);
		}
	}
};



/// The if in code, spawns an ifbranch with two children, the expr and the block of code
class aeNodeEnum : public aeNodeBase
{
public:

	std::string name;
	std::vector<std::string> members;

	aeNodeEnum()
	{
		m_type = Enum;
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
class aeNodeBranch : public aeNodeBase
{
public:
	aeNodeExpr*      expr;
	aeNodeBlock* block;
public:
	aeNodeBranch();
	std::string printtext();
};

class aeNodeReturn : public aeNodeBase
{
public:
	aeNodeReturn()
	{
		m_type = Return;
	}

	std::string printtext()
	{
		return std::string("Return");
	}
};

class aeon_type;

/**
	\class aeNodeUnion
	\brief Each union defined in the code is represented in this structure
*/
class aeNodeUnion : public aeNodeBase
{

};

/**
	\class aeNodeFunction
	\brief All functions are represented by this structure

	This includes:
	- global functions
	- class statics
	- class methods
	- anonymous functions / closures
*/
class aeNodeFunction : public aeNodeBase
{
public:

	std::vector<aeNodeExpr*> m_parameters;
	std::string                   m_name;
	aeNodeTypeDecl*                     m_return;
	aeNodeBlock*                m_block;
	bool                          is_constructor;
	bool                          is_method;
	bool                          is_destructor;
	bool                          is_global;
	bool                          is_static;
	bool                          is_anon;

public:
	aeNodeFunction();

	/// Is this a global function outside class scope
	bool isGlobalFunction();

	/// Is this function a non static method of a class
	bool isNonStaticMethod();

	/// Is this a static class method
	bool isStaticMethod();

	/// Is this is an anonymous function
	bool isAnonymousFunction();

	std::string printtext();
};

class aeNodeWhile : public aeNodeBase
{
public:

	bool           doWhile;
	aeNodeExpr*      expr;
	aeNodeBlock* block;

	aeNodeWhile();

	std::string printtext();
};

class aeNodeFor : public aeNodeBase
{
public:
	std::vector<aeNodeExpr*> initExpressions; ///< The for loop can have any number of init expressions separated by comma.
	aeNodeExpr*      expr;
	aeNodeExpr*      incrExpr;

	aeNodeBlock* block;

	aeNodeFor();

	std::string printtext();
};

class aeNodeUsing : public aeNodeBase
{
public:

	aeNodeExpr* arg = nullptr;

	aeNodeUsing();

	std::string printtext();
};

/*
	has_init_expr
		When true, the compilers will just consider the size of the type for the stack/heap allocation,
		but will leave the job of initializing for the expression.
		When false, the compiler should immediately define the strategy to default construct the var, or
		leave it as-is in case of POD structs.

	name
		The variable name, by which it can be referenced

	type_name
		The type of the declaration, contextual to where the declaration is.
		Default initialization strategy will be deduced after evaluating this type.
*/
class aeNodeVarDecl : public aeNodeStatement
{
public:
	std::string      name;
	std::string      type_name;
	aeNodeExpr* init_expr;

	aeNodeVarDecl();

	std::string printtext();
};

#endif // aeon_tree_h__
