#ifndef AEValue_h__
#define AEValue_h__

#include <AEON/Runtime/AEFunction.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <AEON/AST/aeNodeValue.h>

class AEValueList;
class AEObject;

class AEString
{
public:
	std::string str;
	int ref = 0;
};

class AEArray;

/**
	\class AEValue
	\brief Represents any value any var in aeon can take

	This is mostly used at compilation for assigning complex values into variables,
	a lot like JS and JSON work together.

	Simultaneously, these values can be used to parse standalone data files too.
	The list of supported types:

	Any value can be an object:
	<value> = <opt-type>#<opt-name>{<value...>}
	
	A value can be a literal:
	<value> = 3.4
	<value> = 7
	<value> = "string"

	And a named literal:
	<value> = property: 4.5
	
	And composite values
	<value> = rgba(1.0, 1.0, auto, 1.0)
*/
class AEValue
{
public:

	enum ValueType
	{
		VALUE_UNDEFINED,
		VALUE_OBJECT,
		VALUE_ARRAY,
		VALUE_INT,
		VALUE_REAL,
		VALUE_STRING,
		VALUE_FUNCTION
	};

public:

	AEValue();
	AEValue(std::initializer_list<int> v);
	AEValue(int v);
	AEValue(const std::string& v);
	AEValue(std::function<void()> fn);

	~AEValue();

	ValueType type() const;

	int length() const;

	AEValue at(int index);

	std::string typeName() const;

	void release();

	void setValue(int index, AEValue v);

	void setProperty(const std::string& name, const std::string& value);
	void setProperty(const std::string& name, int value);
	void setProperty(const std::string& name, const AEValue& value);

	AEValue property(const std::string& name) const;
	AEValue property(int index);

	std::string propertyName(int index);

	int numProperties() const;

	void call();
	void call(const AEValueList& argumentList);

	std::string str() const;
	const char* c_str() const;

	bool isCallable();
	bool isUndefined();
	bool isString();

	int asInt();

	static AEValue makeArray();

public:
	
	AEValue& operator=(const AEValue& v);
	AEValue& operator=(const std::string& v);
	AEValue& operator=(int v);
	AEValue& operator=(float v);

	AEValue operator+(AEValue v);
	AEValue operator+(int32_t v);
	AEValue operator+(AEArray& v);
	AEValue operator+(AEString& v);

	operator bool();

private:
	friend class aeParser;

	void setFromArray(AEValue v);
	void setFromString(AEValue v);

	ValueType    m_valueType;

	union {
		double _real;
		int    _int;
		AEString* _string;
		AEObject* _object;
		AEArray*  _array;
		AEFunction* _function;
	};
};

class AEArray
{
public:
	std::vector<AEValue> values;
};


#endif // AEValue_h__
