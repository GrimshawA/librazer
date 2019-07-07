#include <iostream>
#include "program_options_parser.hpp"

#include <RazerRuntime/RzEngine.h>
#include <razer/vm/VirtualMachine.h>
#include <RazerRuntime/AEObject.h>
#include <RazerRuntime/RzDocument.h>
#include <RazerBuild/RzBuilder.h>

#include <RzSDK/ExportStd.h>
#include <razer/vm/Debugger/Debugger.h>

rz::program_options getOpts()
{
    rz::program_options opts;
    opts.addOption("-O[n]", "Level of optimization used for compilation");
    opts.addOption("-I<path>", "Include paths for the compiler to find sources");
    return opts;
}

void showHelp()
{
    std::cout << "Help" << std::endl;
}

int buildAndRun(const std::vector<std::string>& files, const rz::options_results& opts)
{
    RzBuilder::Batch batch;
    for (const auto& file : files)
    {
        std::cout << "Building: " << file << std::endl;
        batch << file;
    }

    if (opts.count("--no-run"))
    {
        std::cout << "Build only mode." << std::endl;
    }

    auto* debugger = new Debugger();

    RzEngine ctx;
    RegisterStd(&ctx);
    ctx.setDebugger(debugger);

    RzBuilder builder(ctx);
    bool result = builder.build(batch);
    if (!result) {
        std::cout << "Build failed" << std::endl;
        return 1;
    }

    ctx.getModule("main")->dumpToFile("code.dmp");

    AEObject* myObject = ctx.createObject("App");
    if (!myObject)
    {
        std::cout << "Failed to find main" << std::endl;
        return 1;
    }

    if (myObject)
    {
        RzVirtualMachine vm(ctx);

        if (opts.count("--dbg"))
        {
            debugger->run();
        }

        vm.m_mainContext.pushObject(myObject);
        vm.call("App.main");

        std::cout << "main() was executed" << std::endl;
    }

    return 0;
}

int main(int argc, char** argv)
{
    auto opts = getOpts();

    if (argc <= 1) {
        std::cout << "No arguments provided.." << std::endl;
        showHelp();
        return 0;
    }

    auto map = rz::parse_cmd_args(opts, argc, argv);

    if (map.count("--help"))
    {
        showHelp();
        return 0;
    }

    return buildAndRun(map.getPositionalArgs(), map);
}
