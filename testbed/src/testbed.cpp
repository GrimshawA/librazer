#include "testbed.h"
#include <Rzr/RzEngine.h>
#include <RazerVM/VirtualMachine.h>
#include <RazerRuntime/AEObject.h>
#include <Rzr/RzDocument.h>
#include <RzrAPI/RzBuilder.h>

#include <iostream>

//#include "../test/test_framework.h"

#include <RazerCore/io/File.h>

void objects_test()
{
	RzEngine ctx;
	ctx.init_all();
	//ctx.quick_build("playground.rz");
	
	RzBuilder builder(ctx);
	builder.build(RzBuilder::Batch()
		<< "arkanoid/main.rz"
		<< "arkanoid/gamecontroller.rz"
		<< "arkanoid/block.rz");

	AEObject* myObject = ctx.createObject("ArkanoidApp");
	if (myObject)
	{
/*		RzValue myObj;
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

		RzValue natObj(myFile, ctx.getTypeInfo("File"));
		*/
		
		RzVirtualMachine vm(&ctx);
		vm.setContext(&ctx);

		//vm.m_stk.push_addr(myObject->m_obj);
		vm.m_mainContext.pushObject(myObject);
		//vm.m_mainContext.pushVariant(myObj);
		//vm.m_mainContext.pushVariant(objB);
		//vm.m_mainContext.pushVariant(objC);
		//vm.m_mainContext.pushVariant(natObj);
		//vm.m_stk.pushVariant(AEValue());
		vm.call(*ctx.modules[0].get(), "ArkanoidApp.main");
	}
}

void testbed::run()
{
	std::cout << "Hey" << std::endl;

	objects_test();

	//system("pause");
}

void main()
{
	testbed t;
	t.run();
}
