#ifndef aeNodeBranch_h__
#define aeNodeBranch_h__

#include <RazerParser/AST/AEBaseNode.h>

class aeNodeExpr;
class aeNodeBlock;

/**
	\class aeNodeBranch
	\brief if-elseif-else

	The primary if node is then followed by the else chain,
	the last branch may or may not have an expression, if it 
	doesn't, its a single else clause.
*/
class aeNodeBranch : public AEStmtNode
{
public:
	std::unique_ptr<aeNodeExpr>                 m_expression;
	std::unique_ptr<aeNodeBlock>                m_block;
	std::vector<std::unique_ptr<aeNodeBranch*>> m_elseChain;

public:
	aeNodeBranch();
    std::string str() const override;
};

#endif // aeNodeBranch_h__
