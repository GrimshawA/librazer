#ifndef aeNodeEcosystem_h__
#define aeNodeEcosystem_h__

#include "AEBaseNode.h"

class RzSourceUnit;

/**
	\class aeNodeEcosystem

	The ecosystem harbors an arbitrary number of modules, it is used to build a complete tree with all modules in the software.
	Allows traversing and other kinds of introspection.
*/
class RzModuleNode : public AEBaseNode
{
public:
	bool checkDuplicates();

	std::vector<std::unique_ptr<RzSourceUnit>> m_modules;
};
#endif // aeNodeEcosystem_h__
