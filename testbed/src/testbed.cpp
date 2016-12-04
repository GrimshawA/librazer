#include "testbed.h"
#include <Rzr/RzEngine.h>
#include <AEON/VM/AEVm.h>
#include <AEON/Runtime/AEObject.h>
#include <AEON/AEDocument.h>
#include <AEON/AEBuilder.h>

#include "../test/test_framework.h"

/*#include "aeBindingHelper.h"
#include "aeon_type_utils.h"

#include <iostream>
#include <aeon_vm.h>
#include <aeon_context.h>
#include <aeon_object.h>
*/
/*void vmexecute(aeon_vm* vm, std::string funcname, aeon_context& context)
{
	vm->setContext(&context);
	vm->call(*context.getModule("main"), funcname.c_str());
}*/
void objects_test()
{
	AEContext ctx;
	ctx.init_all();
	ctx.quick_build("objects.ae");
	//ctx.quick_build("a.ae");

	AEObject* myObject = ctx.createObject("ObjectsTest");
	if (myObject)
	{
		AEValue myObj;
		myObj.setValue("kill", AEValue([](){
			printf("		kill on A\n");
		}));
		myObj.setValue("health", 100);

		myObj.makeRefForChild("health");

		AEValue objB;
		objB.setValue("doThings", AEValue([](){
			printf("           doThings on B\n");
		}));
		//objB.setValue("a", myObj);

		AEValue objC;
		objC.setValue("finish", AEValue([](){
			printf("			FINISH ON C OBJECT\n");
		}));

		//myObject->call("sum2");

		AEVirtualMachine vm;
		vm.setContext(&ctx);

		//vm.m_stk.push_addr(myObject->m_obj);
		vm.m_stk.pushObject(myObject);
		vm.m_stk.pushVariant(myObj);
		vm.m_stk.pushVariant(objB);
		vm.m_stk.pushVariant(objC);
		//vm.m_stk.pushVariant(AEValue());
		vm.call(*ctx.modules[0].get(), "ObjectsTest.sum2");

		printf("FINISH\n");

		//vm.call(AEValue(myObject), "ObjectsTest.sum2", AEValueList() << myObj);
	}

	/*
	AEObject* objA = ctx.createObject("A");
	if (objA)
	{
		printf("OBJ A is valid \n");
		objA->call("do");
	}*/


	//AEDocument document;

	//document.parse("Component { a: 5  }");
	//document.print();

	//document.load("AnimationSample.dmc");
	//document.print();

	//performTests();

	// test builder
	/*aeBuilder b;
	AEBuildSpec bspec;
	bspec.files.push_back("../../demo/sample_app/player.ds");
	bspec.files.push_back("../../demo/sample_app/main.ds");
	b.buildApp("../../demo/myApp.app", bspec);*/

}

void testbed::run()
{
	//std::cout << "Hey" << std::endl;

	objects_test();

	system("pause");
}

void main()
{
	testbed t;
	t.run();
}