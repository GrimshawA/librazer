#ifndef aeNodeWhile_h__
#define aeNodeWhile_h__

#include <RazerParser/AST/AEBaseNode.h>

class aeNodeExpr;
class aeNodeBlock;

class aeNodeWhile : public AEStmtNode
{
public:

	bool           doWhile;
	std::unique_ptr<aeNodeExpr>  expr;
	std::unique_ptr<aeNodeBlock> block;

public:
	aeNodeWhile();

    std::string str() const;
};

#endif // aeNodeWhile_h__
