#ifndef aeNodeNew_h__
#define aeNodeNew_h__

#include "nodes/aeNodeExpr.h"

#include "aeon_tree.h"
#include <vector>
#include <stdint.h>

/**
	\class aeNodeNew
	\brief Represents a new MyClass expression

	It always evaluates to reference to MyClass.
*/
class aeNodeNew : public aeNodeExpr
{
public:
	aeQualType m_instanceType;
};

/*
	Always evaluates to string
*/
class aeNodeNameOf : public aeNodeExpr
{
public:
	std::string m_name;
};

class aeNodeThis : public aeNodeExpr
{
public:
	bool        m_is_implicit;   ///< Was 'this' actually written or inferred from context 
	aeNodeExpr* m_expr;          ///< The expression after the this
};

class aeNodeLiteral : public aeNodeExpr
{
public:

};

class aeNodeString : public aeNodeLiteral
{
public:
	aeNodeString()
	{
		m_nodeType = AEN_STRING;
	}

	virtual std::string exprstr()
	{
		return "\"" + value + "\"";
	}

	std::string printtext()
	{
		return std::string(" string '") + value + "'";
	}

	std::string value;
};


class aeNodeFloat : public aeNodeLiteral
{
public:
	aeNodeFloat()
	{
		m_nodeType = AEN_FLOAT;
	}

	virtual std::string exprstr()
	{
		return std::to_string(value);
	}

	std::string printtext()
	{
		return std::string("Float - ") + std::to_string(value);
	}

	float value;
};

class aeNodeInteger : public aeNodeLiteral
{
public:
	aeNodeInteger()
	{
		m_nodeType = AEN_INTEGER;
	}

	virtual std::string exprstr()
	{
		return std::to_string(value);
	}

	std::string printtext()
	{
		return std::string("Int - ") + std::to_string(value);
	}

	uint32_t value;
};

class aeNodeVarRef : public aeNodeExpr
{
public:

	aeNodeVarRef()
	{
		m_nodeType = AEN_REF;
	}

	bool explicitDeclaration;

	aeQualType VarType;
	std::string m_name;

	virtual aeQualType getQualifiedType(aeon_compiler* c);

	virtual std::string exprstr()
	{
		std::string s1 = m_name;

		if (m_items.size() > 0)
		{
			s1 += "." + static_cast<aeNodeExpr*>(m_items[0])->exprstr();
		}
		return s1;
	}

	std::string printtext()
	{
		return std::string("VarExpr - ") + m_name;
	}
};

#endif // aeNodeNew_h__
