#ifndef aeNodeWhile_h__
#define aeNodeWhile_h__

#include "aeNodeStatement.h"

class aeNodeExpr;
class aeNodeBlock;

class aeNodeWhile : public aeNodeStatement
{
public:

	bool           doWhile;
	aeNodeExpr*      expr;
	std::unique_ptr<aeNodeBlock> block;

	aeNodeWhile();

	std::string printtext();
};

#endif // aeNodeWhile_h__
