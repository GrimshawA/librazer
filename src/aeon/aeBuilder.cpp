#include "aeBuilder.h"
#include <AEON/AEContext.h>
#include <cstdlib>

aeBuilder::aeBuilder(AEContext& context)
: m_context(&context)
{

}

void aeBuilder::addFile(const std::string& sourcefile)
{
	m_sources.push_back(sourcefile);
}

void aeBuilder::build()
{
	printf("------- Build started: %s.\n", m_module.c_str());
	for (std::size_t i = 0; i < m_sources.size(); ++i)
	{
		printf("%s\n", m_sources[i].c_str());
		m_context->quick_build(m_sources[i]);
	}
	printf("------- Build finished.\n");
}

void aeBuilder::buildApp(const std::string& appPath, AEBuildSpec spec)
{
	AEContext ctx;

	for (auto& f : spec.files)
	{
		ctx.quick_build(f);
	}

	FILE* fp = fopen(appPath.c_str(), "wb");
	for (auto& m : ctx.modules)
	{
		fwrite(m->instructions.data(), sizeof(AEInstruction), m->instructions.size(), fp);
	}
	fclose(fp);
	printf("Built application: %s\n", appPath.c_str());
}