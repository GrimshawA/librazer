#include <RzSDK/ExportStd.h>
#include <RzSDK/window/Window.h>
#include <RzSDK/io/File.h>

#include <RzSDK/container/Array.h>
#include <RzSDK/container/String.h>
#include <RzSDK/container/ByteArray.h>

#include <RzSDK/math/Rect.h>

#include <RzSDK/time/Timer.h>

#include <razer/runtime/RzEngine.h>
#include <razer/vm/VirtualMachine.h>
#include <razer/runtime/Generic.h>
#include <razer/frontend/RzBuilder.h>

#include <cmath>
#include <iostream>

void LoadFileToModule(RzModule* m, const std::string& filename)
{
    RzBuilder::Batch batch;
    batch << filename;

    RzBuilder builder(*m->m_context);
    bool result = builder.build(batch);
    if (!result) {
        std::cout << "Build failed" << std::endl;
        return;
    }


}

void RegisterStd(RzEngine* ctx)
{
    /*ctx->registerFunction("void print(int32)", [](RzGeneric g){
		std::string str = g.unpack_string();
		printf("%s\n", str.c_str());
	});

	ctx->registerFunction("void printint(int32)", [](RzGeneric g){
		int32_t v = g.popInt32();
		printf("%d\n", v);
	});

	ctx->registerFunction("void printVar(var)", [](RzGeneric g){
		RzValue v; g.m_threadCtx->popVariant(v);
		printf("VAR: %s\n", v.toString().c_str());
    });*/

	printf("Registered interface!\n");

	RzModule* stdModule = ctx->createModule("std");
	registerFile(stdModule, "File");

    RzTimer::registerApi(stdModule);
    RzByteArray::registerApi(*stdModule);
    RzRect::registerApi(*stdModule);

    ArrayT::registerApi(stdModule);
	Window::registerApi(stdModule);

    LoadFileToModule(stdModule, "Chrono.rz");

    RegisterIO(stdModule);
    RegisterNet(stdModule);
    RegisterWindow(stdModule);
}

void RegisterWindow(RzModule* m)
{
   // m->registerType("Window", sizeof(Window));
}

void RegisterIO(RzModule* m)
{
    registerString(m);
}

void RegisterNet(RzModule* m)
{

}
