#ifndef aeNodeNew_h__
#define aeNodeNew_h__

#include "aeNodeExpr.h"

#include <razer/frontend/AST/Nodes.h>
#include <vector>
#include <stdint.h>

class RzInitBlock;

/**
	\class aeNodeNew
	\brief Represents a new MyClass expression

	It always evaluates to reference to MyClass.
*/
class aeNodeNew : public aeNodeExpr
{
public:
	aeNodeNew();
    explicit aeNodeNew(aeNodeExpr* expr);

    aeNodeExpr* newExpr = nullptr;
    RzInitBlock* initBlock = nullptr;

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
