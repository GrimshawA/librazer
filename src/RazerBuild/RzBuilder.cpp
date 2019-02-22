#include <RazerBuild/RzBuilder.h>
#include <Logger.h>
#include <string>

#include <RazerCompiler/RzCompiler.h>
#include <RazerParser/Parser/RzParser.h>
#include <RazerParser/Parser/TokenParser.h>
#include <RazerParser/AST/RzModuleNode.h>
#include <Base/FileUtils.h>

RzBuilder::RzBuilder(RzEngine& engine)
    : m_engine(engine)
{

}

bool RzBuilder::build(const Batch& b)
{
    RzModule* mainModule = m_engine.createModule("main");
    RzCompiler compiler;
    compiler.m_report = new RzBuildReport();
    compiler.m_env = &m_engine;
    compiler.m_module = mainModule;

    std::vector<RzSourceUnit*> parseTrees;

    RzModuleNode moduleNode;

    // Prepass: collect all parse trees
    for (int i = 0; i < b.files.size(); ++i)
    {
        RzTokenParser lexer;
        RzParser parser;
        std::string source = getFileSource(b.files[i]);

        if (source.empty())
            return false;

        lexer.tokenize(source);

        parser.ctx = &m_engine;
        bool r = parser.startParse(lexer);

        //RzLogger::logToFile("ast.txt", parser.root->str());

        if (!r) {
            RZLOG("Compilation finished with errors.\n", "Cenas");
            return false;
        }

        parseTrees.push_back(parser.root);

        //moduleNode.m_items.emplace_back(parser.root);
        moduleNode.m_modules.emplace_back(parser.root);
    }

    if (!moduleNode.checkDuplicates())
    {
        RZLOG("Compilation finished with errors.\n", "Cenas");
        return false;
    }

    // Collect class information before generating code
    for (int i = 0; i < parseTrees.size(); ++i)
    {
        compiler.collect(*parseTrees[i]);
    }

    // Everything is now available, compile full speed
    for (int i = 0; i < parseTrees.size(); ++i)
    {
        bool r = compiler.generate(parseTrees[i]);
        if (!r)
            return false;
    }

    return true;
}
