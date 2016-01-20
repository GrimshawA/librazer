#ifndef aeQualType_h__
#define aeQualType_h__

class aeon_type;

#include <vector>
#include <string>

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

	aeon_type*  m_type;
	bool        m_const;
	bool        m_ref;
	bool        m_ptr;
	bool        m_volatile;
	bool        m_templated;
	std::vector<aeQualType*> templateTypeArguments;

public:

	/// Initializes to void which means a null type or invalid state
	aeQualType();

	/// Check if the type is a void
	bool isVoid() const;

	/// Returns a formatted string for the type qualifier syntax
	std::string str() const;
};

#endif // aeQualType_h__
