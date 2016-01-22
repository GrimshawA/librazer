#ifndef aeNodeBranch_h__
#define aeNodeBranch_h__

#include "aeNodeStatement.h"

class aeNodeExpr;
class aeNodeBlock;

/**
	\class aeNodeBranch
	\brief if-elseif-else

	The primary if node is then followed by the else chain,
	the last branch may or may not have an expression, if it 
	doesn't, its a single else clause.
*/
class aeNodeBranch : public aeNodeStatement
{
public:
	std::unique_ptr<aeNodeExpr>                 m_expression;
	std::unique_ptr<aeNodeBlock>                m_block;
	std::vector<std::unique_ptr<aeNodeBranch*>> m_elseChain;

public:
	aeNodeBranch();
	std::string printtext();
};

#endif // aeNodeBranch_h__
