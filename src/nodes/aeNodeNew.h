#ifndef aeNodeNew_h__
#define aeNodeNew_h__

#include "aeNodeExpr.h"

#include "../aeon_tree.h"
#include <vector>
#include <stdint.h>

/**
	\class aeNodeNew
	\brief Represents a new MyClass expression

	It always evaluates to reference to MyClass.
*/
class aeNodeNew : public aeNodeExpr
{
public:
	aeQualType m_instanceType;

public:
	aeQualType getQualifiedType(aeon_compiler* c);

};

/*
	Always evaluates to string
*/
class aeNodeNameOf : public aeNodeExpr
{
public:
	std::string m_name;
};

class aeNodeThis : public aeNodeExpr
{
public:
	bool        m_is_implicit;   ///< Was 'this' actually written or inferred from context 
	aeNodeExpr* m_expr;          ///< The expression after the this
};

#endif // aeNodeNew_h__
