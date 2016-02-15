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

	aeQualType VarType;
	std::string m_name;

public:

	aeNodeIdentifier();

	virtual aeQualType getQualifiedType(aeCompiler* c);

	std::string str() const;
};

#endif // aeNodeRef_h__
