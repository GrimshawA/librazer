#ifndef aeNodeBlock_h__
#define aeNodeBlock_h__

#include "aeNodeExpr.h"

class aeNodeBlock : public AEStmtNode
{
public:

	aeNodeBlock();

    std::string str() const override;

	virtual void printSelf(int tabs);
};

#endif // aeNodeBlock_h__
