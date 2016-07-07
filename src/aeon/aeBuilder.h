#ifndef aeBuilder_h__
#define aeBuilder_h__

#include <string>
#include <vector>

class AEContext;

/**
	\class aeBuilder
	\brief Aeon build tool

	The builder takes multiple sources as input
*/
class aeBuilder
{
public:

	/// The source files to be compiled
	std::vector<std::string> m_sources;
	std::string m_module;
	AEContext* m_context;

public:

	aeBuilder(AEContext& context);

	void addFile(const std::string& sourcefile);

	void build();
};

#endif // aeBuilder_h__
