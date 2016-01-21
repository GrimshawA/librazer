#ifndef aeNodeBase_h__
#define aeNodeBase_h__

#include "../aeQualType.h"

#include <vector>
#include <memory>

enum NodeTypes
{
	AEN_ECOSYSTEM,
	AEN_MODULE,
	AEN_NAMESPACE,
	AEN_USING,
	AEN_CLASS,
	AEN_VARDECL,
	AEN_FUNCTION,
	AEN_BLOCK,
	AEN_DIRECTIVE,
	AEN_ACCESSOPERATOR,
	VisibilityMutation,
	CharLiteral,
	StringExpr,
	IntExpr,
	FloatExpr,
	BinaryOperator,
	UnaryOperator,
	AEN_FUNCTIONCALL,
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

class aeNodeBase
{
public:
	
	int                      m_nodeType;
	std::vector<aeNodeBase*> m_items;

public:

	void add(aeNodeBase* n);

	virtual std::string printtext();

	virtual void printSelf(int tabs = 0);
};

#endif // aeNodeBase_h__
