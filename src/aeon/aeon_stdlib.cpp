#include "aeon_stdlib.h"
#include "aeon_context.h"
#include <cmath>

namespace
{
		void atom_cos(atom_generic* s)
		{
			//s->retarg = vmvalue(cosf(s->getArgFloat(0)));
		}
		void atom_sin(atom_generic* s)
		{
			//s->retarg = vmvalue(sinf(s->getArgFloat(0)));
		}

		void atom_print(atom_generic* s)
		{
			std::string str = s->getarg_string(0);
			printf("%s\n", str.c_str());
		}

		void atom_print_int(atom_generic* s)
		{
			int str = s->getarg_int(0);
			printf("%d\n", str);
		}

		void t1(atom_generic*)
		{
			printf("t1()\n");
		}
		void t2(atom_generic* s)
		{
			printf("t2(%f, %f)\n", s->getArgFloat(0), s->getArgFloat(1));
		}
		void t3(atom_generic*)
		{
			printf("t3()\n");
		}
}

void register_stdlib(aeon_context* ctx)
{
	/*ctx->registerFunction("t1", &t1);
	ctx->registerFunction("t2", &t2);
	ctx->registerFunction("t3", &t3);
	ctx->registerFunction("print", &atom_print);
	ctx->registerFunction("pr_int", &atom_print_int);
	ctx->registerFunction("cos", &atom_cos);
	ctx->registerFunction("sin", &atom_sin);*/
}