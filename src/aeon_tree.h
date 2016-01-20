#ifndef aeon_tree_h__
#define aeon_tree_h__

#include "nodes/aeNodeBase.h"
#include "nodes/aeNodeModule.h"
#include "nodes/aeNodeStatement.h"
#include "nodes/aeNodeFunction.h"
#include "nodes/aeNodeFunctionCall.h"
#include "nodes/aeNodeEnum.h"
#include "nodes/aeNodeWhile.h"
#include "nodes/aeNodeFor.h"
#include "nodes/aeNodeBranch.h"
#include "nodes/aeNodeVarDecl.h"
#include "nodes/aeNodeAccessOperator.h"
#include "nodes/aeNodeUnaryOperator.h"
#include "nodes/aeNodeBinaryOperator.h"
#include "nodes/aeNodeTernaryOperator.h"
#include "nodes/aeNodeClass.h"
#include "nodes/aeNodeBlock.h"

#include <string>
#include <vector>
#include <stdint.h>

class aeNodeExpr;
class aeNodeBlock;

static std::string makeTabbing(int tabs)
{
	std::string buff; for (auto k = 0; k < tabs * 2; ++k) buff += ' ';
	return buff;
}

/// The if in code, spawns an ifbranch with two children, the expr and the block of code


class aeNodeTypedef : public aeNodeBase
{
public:

	aeNodeTypedef()
	{
		m_nodeType = Typedef;
	}

	aeQualType* typeA;
	aeQualType* typeB;

	std::string printtext()
	{
		return "Typedef";
	}
};


class aeNodeReturn : public aeNodeBase
{
public:
	aeNodeReturn()
	{
		m_nodeType = Return;
	}

	std::string printtext()
	{
		return std::string("Return");
	}
};

#endif // aeon_tree_h__
