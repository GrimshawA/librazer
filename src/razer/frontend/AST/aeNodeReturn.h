#ifndef aeNodeReturn_h__
#define aeNodeReturn_h__

#include <razer/frontend/AST/AEBaseNode.h>
#include <razer/runtime/QualType.h>

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
