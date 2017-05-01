#include "testbed.h"
#include <RazerRuntime/RzEngine.h>
#include <RazerVM/VirtualMachine.h>
#include <RazerRuntime/AEObject.h>
#include <RazerRuntime/RzDocument.h>
#include <RazerBuild/RzBuilder.h>

#include <iostream>

//#include "../test/test_framework.h"

#include <RazerCore/io/File.h>
#include <STS/CPP/Compiler/CppBuilder.h>

//#include <QDir>
//#include <QDebug>

void run_arkanoid()
{
    //QDir::setCurrent("D:\\Development\\librazer\\testbed\\scripts");
   // qDebug() << QDir::current().absolutePath();

    RzEngine ctx;
    ctx.init_all();

    RzBuilder::Batch batch;
    batch
        << "arkanoid/main.rz";
        //<< "arkanoid/gamecontroller.rz"
        //<< "arkanoid/block.rz";

    /*RzCppBuilder cppBuild(ctx);
    cppBuild.build(batch);*/

    RzBuilder builder(ctx);
    bool result = builder.build(batch);
    if (!result) {
        return;
    }

    AEObject* myObject = ctx.createObject("ArkanoidApp");
    if (!myObject)
    {
        printf("Failed to instance\n");
        return;
    }

    printf("ArkanoidApp size: %d obj: %x\n", myObject->m_type->getSize(), myObject->m_obj);

    if (myObject)
    {
        RzVirtualMachine vm(&ctx);
        vm.setContext(&ctx);

        vm.m_mainContext.pushObject(myObject);
        vm.call("ArkanoidApp.main");
    }

    printf("Finished successfully\n");
}
