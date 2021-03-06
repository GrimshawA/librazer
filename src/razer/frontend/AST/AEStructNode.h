#ifndef AESTRUCTNODE_H__
#define AESTRUCTNODE_H__

#include "AEBaseNode.h"
#include <map>

class RzType;
class aeNodeFunction;
class aeNodeValue;
class RzLetNode;
class aeNodeExpr;
class RzOnNode;

class AEEnumNode : public AEBaseNode
{
public:

	std::string name;
	std::vector<std::string> members;

public:
	AEEnumNode();

	void addField(std::string name);

	std::string str() const;
};

class AEEventNode : public AEBaseNode
{
public:
	std::string name;
	std::vector<RzQualType> params;
};

class AEFieldInitNode : public AEBaseNode
{
public:
	class PropertyInit
	{
		std::string name;
		std::string value;
	};

	std::vector<PropertyInit> inits;

	aeNodeValue* value = nullptr;
};

class AEFieldNode : public AEBaseNode
{
public:
	AEFieldNode();

	std::string visibilityText();

	void deduceStaticType(RzEngine* ctx);

	std::string name;
    aeNodeExpr* declaration = nullptr;

	RzQualType type;
	int visibility;
	AEFieldInitNode* initializer = nullptr;
};

/*
 * Represents named entities within a class body
 * <identifier> : <entity>
 *
 * where entity can be functions, expressions, types, etc
 */
class RzMemberNode : public AEBaseNode
{
public:
	explicit RzMemberNode(const std::string& name, AEBaseNode* ent)
	{
		m_nodeType = AEN_MEMBER;
		identifier = name;
		entity = ent;
	}

	std::string identifier;
	AEBaseNode* entity = nullptr;
};

class AETypeTrait : public AEBaseNode
{
public:
	enum BuiltIn
	{
		TRAIT_NONE,
		TRAIT_PROTO
	};

	BuiltIn m_builtIn = TRAIT_NONE;
};

/**
	\class AEStructNode
	\brief Every struct or class is defined with this structure
*/
class AEStructNode : public AEBaseNode
{
public:

	enum Visibility {
		VISIBILITY_PUBLIC,
		VISIBILITY_PRIVATE,
		VISIBILITY_PROTECTED
	};

	struct classparentinfo
	{
		std::string parentClass;
		Visibility visibility;
	};

	RzType*  m_typeInfo; ///< Runtime type information object for this class ([partials] multiple class nodes may share the same)
	std::string m_name;     ///< The name of the struct/class
	bool        m_struct;   ///< struct or class
	std::vector<classparentinfo> parents;
	std::vector<AETypeTrait> traits;
	std::vector<std::string> templateTypes;
	std::vector<std::unique_ptr<aeNodeFunction>> m_functions;
    std::vector<std::unique_ptr<RzLetNode>> m_bindings;
    std::vector<std::unique_ptr<RzOnNode>> m_onStmts;
	std::vector<AEFieldNode*> m_fields;
	std::vector<AEStructNode> innerClasses;
	std::vector<std::unique_ptr<AEEnumNode>> m_enums;
	std::vector<RzMemberNode*> m_members;
	bool m_interface = false;
	bool m_templated = false;
	bool m_partial = false;

public:
	AEStructNode();

	virtual std::string str() const;

	/// Returns true if the type needs to generate a destructor
	bool generatesDestructor();

	/// Returns the user defined destructor
	aeNodeFunction* getDestructor();

    bool hasField(const std::string& name);

    int getFieldIndex(const std::string& name);

    RzType* getFieldType(const std::string& name);

	/// Check if this is a template class
	bool isTemplated();

	/// Check if this is a generic class
	bool isGeneric();

	/// Check if this is a partial class
	bool isPartial(); 

	/// Returns true if the class needs to generate a default constructor
	bool generatesDefaultConstructor();

	/// Returns true if the class is constructible without parameters
	bool hasDefaultConstructor();

	/// Check if this is an implementable interface
	bool isInterface();

	/// Check if the class supports prototyping
	bool isPrototyped();

	bool adheresToInterface(AEStructNode* intf);

	/// Check if the class has a given method
	bool hasMethod(const std::string& name);

	/// Find a method if it exists
	aeNodeFunction* getMethod(const std::string& name);

	/// Picks the best overload for a given method name, if there is a suitable one
	aeNodeFunction* selectOverload(const std::string& name, std::vector<RzQualType> params);

	/// Create the default constructor for this type
	aeNodeFunction* createDefaultConstructor();

	/// Create the destructor for the type
	aeNodeFunction* createDestructor();
};

class aeNodeIdentifier;

/*
 * Template wrapper node for other entities,
 * Used for template specialization into concrete types
 */
class RzTemplateNode : public AEBaseNode
{
public:
    std::vector<aeNodeIdentifier*> params;
    AEStructNode*                  body;
};

class RzInitBlock : public AEBaseNode
{
public:
    explicit RzInitBlock()
    {
        m_nodeType = AEN_INITBLOCK;
    }
};

#endif // AESTRUCTNODE_H__
