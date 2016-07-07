#ifndef aeNodeWhile_h__
#define aeNodeWhile_h__

#include "aeNodeStatement.h"

class aeNodeExpr;
class aeNodeBlock;

class aeNodeWhile : public aeNodeStatement
{
public:

	bool           doWhile;
	std::unique_ptr<aeNodeExpr>  expr;
	std::unique_ptr<aeNodeBlock> block;

public:
	aeNodeWhile();

	std::string str();
};

#endif // aeNodeWhile_h__