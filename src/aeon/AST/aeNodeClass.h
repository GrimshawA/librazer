#ifndef aeNodeClass_h__
#define aeNodeClass_h__

#include "aeNodeBase.h"

class aeType;
class aeNodeFunction;

/**
	\class aeNodeClass
	\brief Every struct or class is defined with this structure
*/
class aeNodeClass : public aeNodeBase
{
public:

	struct classparentinfo
	{
		std::string parentClass;
		std::string accessLevel;
	};

	aeType*  m_typeInfo; ///< Runtime type information object for this class ([partials] multiple class nodes may share the same)
	std::string m_name;     ///< The name of the struct/class
	bool        m_struct;   ///< struct or class
	std::vector<classparentinfo> parents;
	std::vector<std::unique_ptr<aeNodeFunction>> m_functions;

public:
	aeNodeClass();

	/// Check if the class has a given method
	bool hasMethod(const std::string& name);

	/// Find a method if it exists
	aeNodeFunction* getMethod(const std::string& name);

	/// Create the default constructor for this type
	aeNodeFunction* createDefaultConstructor();

	/// Create the destructor for the type
	aeNodeFunction* createDestructor();
};

#endif // aeNodeClass_h__
