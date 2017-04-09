#include <RazerCore/ExportStd.h>
#include <RazerCore/window/Window.h>
#include <RazerCore/io/File.h>

#include <RazerCore/container/Array.h>
#include <RazerCore/container/String.h>

#include <Rzr/RzEngine.h>
#include <RazerVM/VirtualMachine.h>
#include <RazerRuntime/AEGeneric.h>

#include <cmath>

void RegisterStd(RzEngine* ctx)
{
	ctx->registerFunction("void print(int32)", [](AEGeneric g){
		std::string str = g.unpack_string();
		printf("%s\n", str.c_str());
	});

	ctx->registerFunction("void printint(int32)", [](AEGeneric g){
		int32_t v = g.unpack_int32();
		printf("%d\n", v);
	});

	ctx->registerFunction("void printVar(var)", [](AEGeneric g){
		RzValue v; g.m_threadCtx->popVariant(v);
		printf("VAR: %s\n", v.toString().c_str());
	});

	printf("Registered interface!\n");

	RzModule* stdModule = ctx->createModule("std");
	registerFile(stdModule, "File");

	VariantArray::registerApi(stdModule);
	Window::registerApi(stdModule);

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
