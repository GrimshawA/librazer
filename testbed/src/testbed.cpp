#include "testbed.h"
#include "aeon_type_utils.h"

#include <iostream>
#include <aeon_vm.h>
#include <aeon_context.h>
#include <aeon_object.h>

void vmexecute(aeon_vm* vm, std::string funcname, aeon_context& context)
{
	vm->setContext(&context);
	vm->call(*context.getModule("main"), funcname.c_str());
}

class Widget
{
public:

	~Widget()
	{
		printf("Gone!\n\n");
	}

};

Widget get()
{
	return Widget();
}

void objects_test()
{
	get();
	printf("f");

	aeon_context ctx;
	ctx.init_all();
	ctx.register_type("Widget", sizeof(Widget));
	ctx.quick_build("objects.ae");

	aeon_object* myCoreWidget = ctx.createObject("CoreWidget");
	if (myCoreWidget)
	{
		myCoreWidget->log();
		myCoreWidget->setField("lives", 5);
		myCoreWidget->setField("timeLeft", 10);
		myCoreWidget->setField("myValue", 3550);

		int32_t livesVar = 0;
		myCoreWidget->getField("myValue", livesVar);
		printf("myValue: %d\n", livesVar);

		int32_t timeLeftVar = 0;
		myCoreWidget->getField("timeLeft", timeLeftVar);
		printf("TimeLeftVar %d\n", timeLeftVar);

		printf("\n");
		aeon_vm*     vm = new aeon_vm();
		vm->setContext(&ctx);
		vm->callMethod(myCoreWidget, "execute");

		myCoreWidget->getField("myValue", livesVar);
		printf("myValue: %d\n", livesVar);

		ctx.destroyObject(myCoreWidget);
	}
}

void testbed::run()
{
	std::cout << "Hey" << std::endl;

	objects_test();

	system("pause");
}

void main()
{
	testbed t;
	t.run();
}