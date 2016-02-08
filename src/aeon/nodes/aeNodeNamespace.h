#ifndef aeNodeNamespace_h__
#define aeNodeNamespace_h__

#include "aeNodeBase.h"

#include <map>
#include <memory>

class aeNodeNamespace : public aeNodeBase
{
public:
	std::string m_name; ///< Name of the namespace

public:
	aeNodeNamespace();

	std::string str();
};

#endif // aeNodeNamespace_h__
