#ifndef aeNodeBase_h__
#define aeNodeBase_h__

#include <RazerRuntime/QualType.h>

#include <vector>
#include <memory>

enum NodeTypes
{
	AEN_ECOSYSTEM,
	AEN_MODULE,
	AEN_NAMESPACE,
	AEN_USING,
	AEN_CLASS,
	AEN_VARDECL,
	AEN_FUNCTION,
	AEN_BLOCK,
	AEN_DIRECTIVE,
	AEN_ACCESSOPERATOR,
	AEN_NEW,
	AEN_SUBSCRIPT,
	AEN_VISIBILITY,
	AEN_CHAR,
	AEN_STRING,
	AEN_INTEGER,
	AEN_FLOAT,
	AEN_BINARYOP,
	AEN_UNARYOP,
	AEN_FUNCTIONCALL,
	AEN_BRANCH,
	AEN_WHILE,
	AEN_FOR,
	AEN_RETURN,
	AEN_IDENTIFIER,
	AEN_ENUM,
	AEN_TYPEDEF,
	AEN_OBJECTINIT,
	AEN_IMPORT
};

class AEBaseNode
{
public:
	
	int                      m_nodeType;
	std::vector<AEBaseNode*> m_items;

public:

	void add(AEBaseNode* n);

	virtual std::string str() const;

	virtual void printSelf(int tabs = 0);
};

class AEStmtNode : public AEBaseNode
{
public:
	virtual bool isNullStatement();
};

class aeNodeImport : public AEBaseNode
{
public:
	aeNodeImport()
	{
		m_nodeType = AEN_IMPORT;
	}

	std::string symbol;
};

#endif // aeNodeBase_h__
