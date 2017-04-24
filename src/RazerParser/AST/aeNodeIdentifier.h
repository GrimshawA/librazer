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

    RzQualType getQualifiedType(RzCompiler* c, RzQualType base) override;

	std::string str() const;
};

#endif // aeNodeRef_h__
