#ifndef aeNodeRef_h__
#define aeNodeRef_h__

#include "aeNodeExpr.h"

#include <vector>
#include <stdint.h>

/**
	\class aeNodeRef
	\brief Reference to a stored variable
*/
class aeNodeIdentifier : public aeNodeExpr
{
public:
	bool explicitDeclaration;

	RzQualType VarType;
	std::string m_name;

public:

	aeNodeIdentifier();
    explicit aeNodeIdentifier(const std::string& name);

	std::string str() const;
};

/*
 * TODO: Find better name
 *
 * This represents something templated, like my_type<T>
 *
 * For example a.b would be a member access expr with two identifiers
 * a<T>.b would be a member access with a lhs construct expr and identifier rhs
 */
class RzConstructExpr : public aeNodeExpr
{
public:
    RzConstructExpr()
    {
        m_nodeType = AEN_CONSTRUCT;
    }
    virtual ~RzConstructExpr() = default;

    std::string str() const;

    aeNodeExpr* base = nullptr;
    aeNodeExpr* param = nullptr; // single param for now
};

#endif // aeNodeRef_h__
