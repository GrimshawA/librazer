#ifndef aeNodeBase_h__
#define aeNodeBase_h__

#include <razer/runtime/QualType.h>
#include <razer/ir/ir_builder.hpp>

#include <vector>
#include <memory>

enum NodeTypes
{
	AEN_ECOSYSTEM,
	AEN_MODULE,
	AEN_NAMESPACE,
	AEN_USING,
	AEN_CLASS,
	AEN_MEMBER,
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
    AEN_RANGEFOR,
	AEN_RETURN,
	AEN_IDENTIFIER,
    AEN_CONSTRUCT,
	AEN_ENUM,
	AEN_TYPEDEF,
	AEN_OBJECTINIT,
    AEN_IMPORT,
    AEN_ATTRIBUTE,
    AEN_INITBLOCK
};

class AEBaseNode
{
public:

    virtual IRValue* emitIR(IRBuilder& builder) {return nullptr;}
	
    void prepend(AEBaseNode* node)
    {
        m_items.insert(m_items.begin(), node);
    }

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

class RzAttributeNode : public AEBaseNode
{
public:
    RzAttributeNode()
    {
        m_nodeType = AEN_ATTRIBUTE;
    }

    RzAttributeNode(const std::string& text)
    {
        m_nodeType = AEN_ATTRIBUTE;
        contents = text;
    }

    std::string contents;
};

#endif // aeNodeBase_h__
