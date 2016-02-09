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

	virtual std::string exprstr()
	{
		return "\"" + value + "\"";
	}

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

	virtual std::string exprstr()
	{
		return ".";
		//return std::to_string(value);
	}

	std::string str() const
	{
		return std::to_string(value);
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
		return ";";
		//return std::to_string(value);
	}

	std::string str() const
	{
		return std::to_string(value);
	}

	uint32_t value;
};

#endif // aeNodeLiterals_h__
