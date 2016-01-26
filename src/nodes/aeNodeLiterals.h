#ifndef aeNodeLiterals_h__
#define aeNodeLiterals_h__

#include "aeNodeExpr.h"




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

	std::string str()
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

	std::string str()
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

	std::string str()
	{
		return std::string("Int - ") + std::to_string(value);
	}

	uint32_t value;
}; 

#endif // aeNodeLiterals_h__
