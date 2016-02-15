#ifndef aeNodeLiterals_h__
#define aeNodeLiterals_h__

#include "aeNodeExpr.h"
#include <string>



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

	aeQualType getQualifiedType(aeCompiler* c);


	std::string str() const
	{
		return "\"" + value + "\"";
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

	std::string str() const
	{
		return std::to_string(value);
	}

	aeQualType getQualifiedType(aeCompiler* c);


	float value;
};

class aeNodeInteger : public aeNodeLiteral
{
public:
	aeNodeInteger()
	{
		m_nodeType = AEN_INTEGER;
	}

	aeQualType getQualifiedType(aeCompiler* c);


	std::string str() const
	{
		return std::to_string(value);
	}

	uint32_t value;
};

#endif // aeNodeLiterals_h__
