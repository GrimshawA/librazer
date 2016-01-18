#ifndef aeNodeClass_h__
#define aeNodeClass_h__

#include "aeNodeBase.h"

class aeon_type;

/**
	\class aeNodeClass
	\brief Every struct or class is defined with this structure
*/
class aeNodeClass : public aeNodeBase
{
public:

	aeon_type*  m_typeInfo; ///< Runtime type information object for this class ([partials] multiple class nodes may share the same)
	std::string m_name;     ///< The name of the struct/class
	bool        m_struct;   ///< struct or class

public:

	struct classparentinfo
	{
		std::string parentClass;
		std::string accessLevel;
	};

	std::vector<classparentinfo> parents;

	aeNodeClass();
};

#endif // aeNodeClass_h__
