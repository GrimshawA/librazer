#ifndef aeBuilder_h__
#define aeBuilder_h__

#include <string>
#include <vector>

class AEContext;

class AEBuildSpec
{
public:
	std::vector<std::string> files;
};

/**
	\class aeBuilder
	\brief Aeon build tool

	The builder takes multiple sources as input

	Builds libraries and applications into binaries or runtime code.
*/
class aeBuilder
{
public:

	/// The source files to be compiled
	std::vector<std::string> m_sources;
	std::string m_module;
	AEContext* m_context;

public:
	aeBuilder(){}
	aeBuilder(AEContext& context);

	void addFile(const std::string& sourcefile);

	void build();

	void buildApp(const std::string& appPath, AEBuildSpec spec);
};

#endif // aeBuilder_h__
