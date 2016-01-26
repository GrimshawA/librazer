#ifndef aeNodeVarDecl_h__
#define aeNodeVarDecl_h__

#include "aeNodeStatement.h"

class aeNodeExpr;

/*
	\class aeNodeVarDecl
	\brief Represents initialization of one or more variables of the same type

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
	struct Decl
	{
		std::string                 m_name;
		//std::unique_ptr<aeNodeExpr> m_initExpr;
	};

	aeQualType        m_type;
	std::vector<Decl> m_decls;

public:

	aeNodeVarDecl();

	std::string str() const;
};

#endif // aeNodeVarDecl_h__
