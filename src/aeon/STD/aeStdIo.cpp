#include <AEON/STD/aeStdIo.h>
#include <AEON/AEContext.h>
#include <AEON/VM/AEVm.h>
#include <AEON/Runtime/AEGeneric.h>

#include <cmath>

#include <AEON/stdlib/io/File.h>

class MySprite
{
	int a, b, c;
};

void register_stdlib(AEContext* ctx)
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
		AEValue v; g.m_vm->m_stk.popVariant(v);
		printf("VAR: %s\n", v.toString().c_str());
	});

	ctx->registerType("Sprite", sizeof(MySprite));

	printf("Registered interface!\n");

	AEModule* stdModule = ctx->createModule("std");
	registerFile(stdModule, "File");
}