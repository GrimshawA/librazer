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

	float value;
};

class aeNodeInteger : public aeNodeLiteral
{
public:
	aeNodeInteger()
	{
		m_nodeType = AEN_INTEGER;
	}

	std::string str() const
	{
		return std::to_string(value);
	}

	uint32_t value;
};

/*
	An array literal in the code, as a RVALUE to assign on something.
	a: [0,1,2,3,4]
	myArray = [0,1,2,3,4]

	inside code or property definitions.
*/
class aeNodeArray : public aeNodeLiteral
{
public:
	
	void addElement(aeNodeValue* elem)
	{
		m_elements.push_back(std::unique_ptr<aeNodeValue>(elem));
	}

	std::string str() const
	{
		std::string s = "[";
		for (int i = 0; i < m_elements.size(); ++i){
			s += m_elements[i]->str();
			if (i < m_elements.size() - 1)
				s += ",";
		}
		s += "]";
		return s;
	}

private:
	std::vector<std::unique_ptr<aeNodeValue>> m_elements;
};

#endif // aeNodeLiterals_h__
