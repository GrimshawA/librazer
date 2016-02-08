#ifndef aeNodeBase_h__
#define aeNodeBase_h__

#include "../aeQualType.h"

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
	AEN_REF,
	AEN_ENUM,
	AEN_TYPEDEF,
};

class aeNodeBase
{
public:
	
	int                      m_nodeType;
	std::vector<aeNodeBase*> m_items;

public:

	void add(aeNodeBase* n);

	virtual std::string str() const;

	virtual void printSelf(int tabs = 0);
};

#endif // aeNodeBase_h__
