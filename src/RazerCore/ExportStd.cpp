#include <RazerCore/ExportStd.h>
#include <RazerCore/window/Window.h>
#include <RazerCore/io/File.h>

#include <RazerCore/container/Array.h>

#include <Rzr/RzEngine.h>
#include <RazerVM/VirtualMachine.h>
#include <RazerRuntime/AEGeneric.h>

#include <cmath>

class MySprite
{
	int a, b, c;
};

class TestClass {
public:

		int x;
		int y;

		void setSomething(){}

};

void aeonTestClass_setSomething(AEGeneric g){

}

void RegisterStd(RzEngine* ctx)
{
	/*ctx->registerFunction("t1", &t1);
	ctx->registerFunction("t2", &t2);
	ctx->registerFunction("t3", &t3);
	ctx->registerFunction("print", &atom_print);
	ctx->registerFunction("pr_int", &atom_print_int);
	ctx->registerFunction("cos", &atom_cos);
	ctx->registerFunction("sin", &atom_sin);*/

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

	stdModule->registerType("TestClass", sizeof(TestClass));
	stdModule->registerMethod("TestClass", "setSomething", aeonTestClass_setSomething);

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

}

void RegisterNet(RzModule* m)
{

}
