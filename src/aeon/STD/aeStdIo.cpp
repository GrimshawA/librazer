#include <AEON/STD/aeStdIo.h>
#include <AEON/Runtime/AEContext.h>
#include <AEON/Runtime/AEVm.h>

#include <cmath>

void register_stdlib(AEContext* ctx)
{
	/*ctx->registerFunction("t1", &t1);
	ctx->registerFunction("t2", &t2);
	ctx->registerFunction("t3", &t3);
	ctx->registerFunction("print", &atom_print);
	ctx->registerFunction("pr_int", &atom_print_int);
	ctx->registerFunction("cos", &atom_cos);
	ctx->registerFunction("sin", &atom_sin);*/

	ctx->registerFunction("void print(int32)", [](aeVM* vm){
		vm_value strIndex = vm->m_stk.pop_value();
		std::string str = vm->m_ctx->string_literals[strIndex.i32];
		printf("%s\n", str.c_str());
	});

	printf("Registered interface!\n");
}