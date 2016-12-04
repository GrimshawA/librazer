#ifndef aeNodeReturn_h__
#define aeNodeReturn_h__

#include <AEON/AST/AEBaseNode.h>
#include "../Runtime/aeQualType.h"

#include <vector>
#include <memory>

class aeNodeExpr;

class aeNodeReturn : public AEStmtNode
{
public:
	aeNodeExpr* m_expression;

public:

	aeNodeReturn();

	std::string str();
};

#endif // aeNodeReturn_h__
