#include <STS/Cpp/Compiler/CppBuilder.h>
#include <RazerParser/Parser/RzParser.h>
#include <Base/FileUtils.h>

RzCppBuilder::RzCppBuilder(RzEngine& ctx)
	: m_ctx(ctx)
{

}

void RzCppBuilder::build(RzBuilder::Batch batch)
{
	std::vector<std::shared_ptr<RzSourceUnit>> sourceTrees;
	std::vector<std::string> fileNames;

	for(int i = 0; i < batch.files.size(); ++i)
	{
		std::string source = getFileSource(batch.files[i]);
		std::shared_ptr<RzSourceUnit> parseTree = RzParser::getParseTree(source, m_ctx);

		sourceTrees.push_back(parseTree);
		fileNames.push_back(rzGetFileName(batch.files[i]));
	}

	int i = 0;
	for(std::shared_ptr<RzSourceUnit> tree : sourceTrees)
	{
		generateCppUnit(fileNames[i++], tree);
	}
}

void RzCppBuilder::generateCppUnit(const std::string& name, std::shared_ptr<RzSourceUnit> root)
{
	printf("Generating file!\n");

	rzCreateDir("CppMod");


	std::string headerFile = "CppMod/" + name + ".h";
	std::string sourceFile = "CppMod/" + name + ".cpp";

	FILE* fp = fopen(headerFile.c_str(), "w");
	FILE* fp2 = fopen(sourceFile.c_str(), "w");

	printf("Created code %s\n", sourceFile.c_str());
}
