#ifndef aeNodeVarDecl_h__
#define aeNodeVarDecl_h__

#include "aeNodeStatement.h"

class aeNodeExpr;

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
	std::string                 m_name;
	aeQualType                  m_type;
	std::unique_ptr<aeNodeExpr> m_initExpr;

public:

	aeNodeVarDecl();

	std::string printtext() const;
};

#endif // aeNodeVarDecl_h__
