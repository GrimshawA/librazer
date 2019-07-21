#ifndef RZVALUE_H__
#define RZVALUE_H__

#include <RazerRuntime/Function.h>
#include <RazerRuntime/AEString.h>

#include <string>
#include <vector>
#include <memory>
#include <functional>

#include <razer/frontend/AST/aeNodeValue.h>

class AEValueList;
class AEObject;
class AEDynamicObject;
class AEArray;
class RzValue;

class AEValueRef {
public:

	enum RefType
	{
		REF_UNDEFINED,
		REF_VALUE,
		REF_RAW
	};

	union {
		RzValue* value;
		void*    data;
	};

	RefType type;
	RzValue* container;

	bool createOnAssign();
};

/**
	\class RzValue
	\brief Variant type that can assume any value

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
class RzValue
{
public:

	enum ValueType
	{
		VALUE_UNDEFINED,
		VALUE_OBJECT,
		VALUE_PROTOTYPE,
		VALUE_ARRAY,
		VALUE_INT,
		VALUE_REAL,
		VALUE_STRING,
		VALUE_FUNCTION
	};

public:

	RzValue();
	RzValue(void* obj, RzType* typeInfo); ///< Constructs from arbitrary object and its type information
	RzValue(std::initializer_list<int> v);
	RzValue(int v);
	RzValue(const std::string& v);
	RzValue(std::function<void()> fn);
	RzValue(AEObject* obj);

	~RzValue();

	ValueType type() const;

	int length() const;

	RzValue at(int index);

	std::string typeName() const;

	void release();

	void setValue(int index, RzValue v);
	void setValue(const std::string& name, const std::string& value);
	void setValue(const std::string& name, int value);
	void setValue(const std::string& name, const RzValue& value);

	RzValue property(const std::string& name) const;
	RzValue property(int index);

	std::string propertyName(int index);

	int numProperties() const;

	void call();
	void call(const AEValueList& argumentList);

	std::string str() const;
	const char* c_str() const;

	bool isCallable();
	bool isUndefined();
	bool isString();

	int toInteger();
	std::string toString() const;
	bool toBoolean() const;

	static RzValue makeArray();

	AEValueRef makeRef();
	AEValueRef makeRefForChild(const std::string& name);

public:
	
	RzValue& operator=(const RzValue& v);
	RzValue& operator=(const std::string& v);
	RzValue& operator=(int v);
	RzValue& operator=(float v);

public:
	/* Variant operations */
	RzValue operator+(const RzValue& b);
	RzValue operator-(const RzValue& b);
	RzValue operator*(const RzValue& b);
	RzValue operator/(const RzValue& b);

	bool    operator==(const RzValue& b);

public:

	RzValue operator+(int32_t v);
	RzValue operator+(AEArray& v);
	RzValue operator+(AEString& v);

	operator bool();

	void setFromObject(void* ptr, RzType* typeInfo);

//private:
	friend class RzParser;

	void setFromArray(RzValue v);
	void setFromString(RzValue v);

	ValueType    m_valueType;

	union {
		double _real;
		int    _int;
		AEString* _string;
		AEObject* _object;
		AEDynamicObject* _dynObj;
		AEArray*  _array;
		RzFunction* _function;
	};
};

class AEArray
{
public:
	std::vector<RzValue> values;
};

#endif // RZVALUE_H__
