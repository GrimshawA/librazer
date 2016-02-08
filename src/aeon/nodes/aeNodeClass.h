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

	aeType*  m_typeInfo; ///< Runtime type information object for this class ([partials] multiple class nodes may share the same)
	std::string m_name;     ///< The name of the struct/class
	bool        m_struct;   ///< struct or class

public:

	struct classparentinfo
	{
		std::string parentClass;
		std::string accessLevel;
	};

	std::vector<classparentinfo> parents;

public:
	aeNodeClass();

	/// Check if the class has a given method
	bool hasMethod(const std::string& name);

	/// Find a method if it exists
	aeNodeFunction* getMethod(const std::string& name);
};

#endif // aeNodeClass_h__
