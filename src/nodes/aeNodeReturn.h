#ifndef aeNodeReturn_h__
#define aeNodeReturn_h__

#include "aeNodeStatement.h"
#include "../aeQualType.h"

#include <vector>
#include <memory>

class aeNodeExpr;

class aeNodeReturn : public aeNodeStatement
{
public:
	aeNodeExpr* m_expression;

public:

	aeNodeReturn();

	std::string str();
};

#endif // aeNodeReturn_h__
