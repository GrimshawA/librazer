#ifndef STSCPPBUILDER_H__
#define STSCPPBUILDER_H__

#include <RazerBuild/RzBuilder.h>
#include <RazerParser/AST/RzSourceUnit.h>
#include <memory>

class RzEngine;

class RzCppBuilder
{
public:
	RzCppBuilder(RzEngine& ctx);

	void build(RzBuilder::Batch batch);

	void generateCppUnit(const std::string& name, std::shared_ptr<RzSourceUnit> root);

private:
	RzEngine& m_ctx;
};

#endif
