#ifndef aeBuilder_h__
#define aeBuilder_h__

#include <string>
#include <vector>

class RzEngine;

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
	RzEngine* m_context;

public:
	aeBuilder(){}
	aeBuilder(RzEngine& context);

	void addFile(const std::string& sourcefile);

	void build();

	void buildApp(const std::string& appPath, AEBuildSpec spec);
};

#endif // aeBuilder_h__
