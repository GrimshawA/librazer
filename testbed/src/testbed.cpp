#include "testbed.h"
#include <Rzr/RzEngine.h>
#include <RazerVM/VirtualMachine.h>
#include <AEON/Runtime/AEObject.h>
#include <Rzr/RzDocument.h>
#include <AEON/Build/aeBuilder.h>

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
	RzEngine ctx;
	ctx.init_all();
	ctx.quick_build("playground.rz");
	//ctx.quick_build("a.ae");

	AEObject* myObject = ctx.createObject("ObjectsTest");
	if (myObject)
	{
		RzValue myObj;
		myObj.setValue("kill", RzValue([](){
			printf("		kill on A\n");
		}));
		myObj.setValue("health", 100);

		myObj.makeRefForChild("health");

		RzValue objB;
		objB.setValue("doThings", RzValue([](){
			printf("           doThings on B\n");
		}));
		//objB.setValue("a", myObj);

		RzValue objC;
		objC.setValue("finish", RzValue([](){
			printf("			FINISH ON C OBJECT\n");
		}));

		//myObject->call("sum2");

		RzVirtualMachine vm(&ctx);
		vm.setContext(&ctx);

		//vm.m_stk.push_addr(myObject->m_obj);
		vm.m_mainContext.pushObject(myObject);
		vm.m_mainContext.pushVariant(myObj);
		vm.m_mainContext.pushVariant(objB);
		vm.m_mainContext.pushVariant(objC);
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