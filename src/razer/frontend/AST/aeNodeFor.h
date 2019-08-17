#ifndef aeNodeFor_h__
#define aeNodeFor_h__

#include <razer/frontend/AST/AEBaseNode.h>
#include <razer/frontend/AST/aeNodeIdentifier.h>

class aeNodeExpr;
class aeNodeBlock;

class RzForBaseNode : public AEStmtNode
{

};

class aeNodeFor : public RzForBaseNode
{
public:
	std::unique_ptr<AEStmtNode> initStatement; ///< The for loop can have any number of init expressions separated by comma.
	std::unique_ptr<aeNodeExpr>      expr;
	std::unique_ptr<aeNodeExpr>      incrExpr;
	std::unique_ptr<aeNodeBlock>     block;

public:
	aeNodeFor();

	std::string str() const;
};

class RzRangeForNode : public RzForBaseNode
{
public:
    std::unique_ptr<aeNodeExpr> iterator;
    std::unique_ptr<aeNodeExpr>       range;
    std::unique_ptr<aeNodeBlock>      block;

public:
    RzRangeForNode();
    RzRangeForNode(aeNodeExpr* identity, aeNodeExpr* range);

    std::string str() const;
};

#endif // aeNodeFor_h__
