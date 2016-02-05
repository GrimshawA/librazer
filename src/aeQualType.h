#ifndef aeQualType_h__
#define aeQualType_h__

class aeon_type;

#include <vector>
#include <stdint.h>
#include <string>

enum AEPrimitive
{
	AE_PFLOAT,
	AE_PDOUBLE,
	AE_INT32,
	AE_UINT32,
	AE_PTR,
	AE_REF,
};

/**
	\class aeQualType
	\brief A qualified type is the sum of a base declared type with additional qualifiers

	These qualifiers include templating, const, volatile and other useful information.
	Every qualified type DEPENDS on a defined type or primitive.

	For example:
	const int => aeQualType dependent on int
	const MyClass& => aeQualType dependent on a MyClass type
	MyArray<int> => aeQualType dependent on a MyArray type, which accepts template arguments
*/
class aeQualType
{
public:

	aeon_type*              m_type;
	bool                    m_const;
	bool                    m_ref;
	bool                    m_ptr;
	bool                    m_volatile;
	bool                    m_templated;
	std::vector<aeQualType> m_templateArgs;

public:

	/// Generates a fully qualified type from a string describing it
	static aeQualType fromString(const std::string& str);

public:

	/// Initializes to void which means a null type or invalid state
	aeQualType();

	/// Get the static size of this type
	uint32_t getSize();

	/// Get the number of template arguments in the type
	uint32_t getNumTemplateArgs() const;

	/// Get the template arguments
	aeQualType getTemplateArg(uint32_t index) const;

	/// Returns the type name (or "void")
	std::string getName() const;

	/// Get the type information object
	aeon_type* getType() const;

	/// Check if the type is immutable
	bool isConst() const;

	/// Check if the type instantiates a template or generic
	bool isTemplated() const;

	/// Check if this type is a primitive
	bool isPrimitive() const;

	/// Check if the type is a void
	bool isVoid() const;

	/// Get the primitive type in an enum
	AEPrimitive getPrimitive() const;

	/// Returns a formatted string for the type qualifier syntax
	std::string str() const;

	/// Checks for validity
	operator bool() const;
};

#endif // aeQualType_h__
