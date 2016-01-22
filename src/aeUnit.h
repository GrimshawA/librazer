#ifndef aeUnit_h__
#define aeUnit_h__

#include <string>
#include <vector>

/**
	\class aeUnit
	\brief Represents one source code file during runtime

	This acts as a logical containment unit for a source code file during runtime.
	For example, all global functions and types go to the module scope, where they are
	managed. The aeUnit tracks entities that were defined in a given source file
	so they can be logically separated by the language user.

	This is more of a tool for third party users of the runtime, and it usually
	remains hidden for users of the language, who should think at the module level.
*/
class aeUnit
{
public:
	std::string m_path;
	std::vector<int> m_functions;
	std::vector<int> m_classes;
};

#endif // aeUnit_h__
