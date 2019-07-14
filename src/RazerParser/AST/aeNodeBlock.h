#ifndef aeNodeBlock_h__
#define aeNodeBlock_h__

#include "aeNodeExpr.h"

class IRBuilder;

class aeNodeBlock : public AEStmtNode
{
public:

	aeNodeBlock();

    IRValue* emitIR(IRBuilder& builder);

    std::string str() const override;

	virtual void printSelf(int tabs);
};

#endif // aeNodeBlock_h__
