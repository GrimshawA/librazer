#include "testbed.h"
#include "aeBindingHelper.h"
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

class GameObject
{
public:
	int numComponents;
	int health;
	float mExperience;

	GameObject()
	{
		static int i = 0;
		i++;
		printf("GameObject(%d) constructed at %x\n",i, this);
	}

	~GameObject()
	{
		printf("~GameObject(BYE)\n");
	}

	void setHealth(int  health)
	{
		numComponents = health; 
		printf("health changed\n");
	}
};

void GameObject_Constructor(aeon_vm* vm)
{
	void* memory = vm->m_stk.pop_value().ptr;
	printf("init GO on %x\n", memory);
	new (memory) GameObject();
}

void GameObject_Destructor(aeon_vm* vm)
{
	void* memory = vm->m_stk.pop_value().ptr;
	static_cast<GameObject*>(memory)->~GameObject();
}

void GameObject_setHealth(aeon_vm* vm)
{
	void* memory = vm->m_stk.pop_value().ptr;
	printf("HEALTH GO on %x\n", memory);
	static_cast<GameObject*>(memory)->setHealth(100);
	printf(">>>>>>>>>Called setHealth wrap\n");
}

class Widget
{
public:

	~Widget()
	{
		printf("Gone!\n\n");
	}

};


void objects_test()
{
	aeon_context ctx;
	ctx.init_all();
	ctx.registerType("Widget", sizeof(Widget));
	ctx.registerType("GameObject", sizeof(GameObject));
	ctx.registerTypeBehavior("GameObject", "f", &GameObject_Constructor);
	ctx.registerTypeBehavior("GameObject", "~f", &GameObject_Destructor);
	ctx.registerTypeMethod("GameObject", "setHealth", &GameObject_setHealth);
	ctx.registerTypeField("GameObject", "m_health", aeOFFSET(GameObject, health));

	ctx.quick_build("objects.ae");

	aeon_object* myCoreWidget = ctx.createObject("CoreWidget");
	if (myCoreWidget)
	{
		myCoreWidget->log();
		myCoreWidget->setField("lives", 5);
		myCoreWidget->setField("timeLeft", 10);
		myCoreWidget->setField("myValue", 3550);
		myCoreWidget->setField("myOtherValue", 0);
		printf("Target var absolute addr %x\n", myCoreWidget->getFieldAddress("myValue"));

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

		int32_t myOtherValue;
		myCoreWidget->getField("myOtherValue", myOtherValue);
		printf("myOtherValue %d\n", myOtherValue);

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