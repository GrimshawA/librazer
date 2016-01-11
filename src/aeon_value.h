#ifndef aeon_value_h__
#define aeon_value_h__

#include <string>
#include <vector>

/**
	\class aeon_value
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
class aeon_value
{
public:

		enum ValueType
		{
			EInvalid,
			EObject,
			EArray,
			EInteger,
			EString,
		};

		ValueType mValueType;
		std::string mRawValue;
		std::string mRawProperty;

		/// Child items
		std::vector<aeon_value> properties;

	public:

		/// Construct default empty object
		aeon_value();

		aeon_value* begin();

		aeon_value* end();

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
		aeon_value operator[](const std::string& str);
};

#endif // aeon_value_h__
