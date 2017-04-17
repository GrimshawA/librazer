#ifndef aeNodeModule_h__
#define aeNodeModule_h__

#include "AEBaseNode.h"

#include <map>
#include <memory>

class AEStructNode;
class aeNodeNamespace;

/**
	\class aeNodeModule
	\brief A module groups many language constructs together in independent groups

	Each module will potentially define:
	- Types
	- Type aliases
	- Global objects
	- Namespaces

	Modules can naturally reference one another, as a way to group functionality together.
*/
class RzSourceUnit : public AEBaseNode
{
public:

	typedef std::unique_ptr<aeNodeNamespace> aeNodeNamespaceObj;
	typedef std::unique_ptr<AEStructNode> aeNodeClassObj;

	std::string                        m_name;       ///< Name of the module (can be empty)
	std::map<std::string, std::string> m_directives; ///< Configuration directives for this module
	std::vector<aeNodeNamespaceObj>    m_namespaces; ///< Namespaces defined within the module
	std::vector<aeNodeClassObj>        m_types;      ///< All structs and classes defined at top level
public:

	RzSourceUnit();

    std::string str() const override;
	virtual void printSelf(int tabs);
};

#endif // aeNodeModule_h__
