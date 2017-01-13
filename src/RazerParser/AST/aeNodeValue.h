#ifndef aeNodeValue_h__
#define aeNodeValue_h__

#include <RazerParser/AST/AEBaseNode.h>

class aeNodeExpr;
class aeNodeBlock;

class aeNodeValue : public AEStmtNode
{
public:

};

class AEObjectInitNode : public aeNodeValue
{
public:

	AEObjectInitNode(const std::string& typeName)
	{
		m_typename = typeName;
		m_nodeType = AEN_OBJECTINIT;
	}

	std::string m_typename;
};

#endif // aeNodeValue_h__
