#ifndef aeNodeNew_h__
#define aeNodeNew_h__

#include "aeNodeExpr.h"

#include <RazerParser/AST/Nodes.h>
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
	aeNodeNew();

	RzQualType m_instanceType;
	std::string type;
};

/*
	Always evaluates to string
*/
class aeNodeNameOf : public aeNodeExpr
{
public:
	std::string m_name;
};

#endif // aeNodeNew_h__
