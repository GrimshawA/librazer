#ifndef AEValue_h__
#define AEValue_h__

#include <string>
#include <vector>
#include <memory>

#include <AEON/AST/aeNodeValue.h>

class AEValueList;

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
	~AEValue();

	void release();

	void setProperty(const std::string& name, const std::string& value);
	void setProperty(const std::string& name, int value);
	void setProperty(const std::string& name, const AEValue& value);

	AEValue property(const std::string& name);

	void call();
	void call(const AEValueList& argumentList);

	bool isCallable();
	bool isUndefined();
	bool isString();

public:
	
	AEValue& operator=(const AEValue& v);
	AEValue& operator=(const std::string& v);
	AEValue& operator=(int v);


		AEValue* begin();
		AEValue* end();

		/// Creates a string property on this object
		void CreateString(const std::string& name, const std::string& value);

		/// Creates a float on this object
		void CreateFloat(const std::string& name, float value);

		/// Create a number, of any type
		void CreateNumber(const std::string& name, const std::string& number);

		/// Sets this value as a number
		void SetNumber(const std::string& name, const std::string& number);

		/// Sets this value as a number
		void SetString(const std::string& name, const std::string& value);

		/// Check if this value is an object
		/// It is an object as long as it has more than one property
		bool IsObject();

		void debugPrint();

		int as_int();

		float as_float();

		std::string as_string();

		/// Read a property of this value
		AEValue operator[](const std::string& str);

private:
	friend class aeParser;

	ValueType m_valueType;
	void*     m_data;
};

#endif // AEValue_h__
