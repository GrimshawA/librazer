#ifndef aeNodeFor_h__
#define aeNodeFor_h__

#include "aeNodeStatement.h"

class aeNodeExpr;
class aeNodeBlock;

class aeNodeFor : public aeNodeStatement
{
public:
	std::unique_ptr<aeNodeStatement> initStatement; ///< The for loop can have any number of init expressions separated by comma.
	std::unique_ptr<aeNodeExpr>      expr;
	std::unique_ptr<aeNodeExpr>      incrExpr;
	std::unique_ptr<aeNodeBlock>     block;

public:
	aeNodeFor();

	std::string str() const;
};

#endif // aeNodeFor_h__
