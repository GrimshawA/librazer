#include <src/run_pong.h>
#include <Rzr/RzEngine.h>
#include <RazerVM/VirtualMachine.h>
#include <RazerRuntime/AEObject.h>
#include <Rzr/RzDocument.h>
#include <RzrAPI/RzBuilder.h>

#include <iostream>

#include <RazerCore/io/File.h>
#include <STS/CPP/Compiler/CppBuilder.h>

#include <QDir>
#include <QDebug>

void run_pong_game() {
    QDir::setCurrent("D:\\Development\\librazer\\testbed\\scripts");
    qDebug() << QDir::current().absolutePath();

    RzEngine ctx;
    ctx.init_all();

    RzBuilder::Batch batch;
    batch
        << "pong/PongGame.rz";

    RzBuilder builder(ctx);
    bool result = builder.build(batch);
    if (!result) {
        return;
    }

    AEObject* myObject = ctx.createObject("PongGame");
    if (!myObject)
    {
        qDebug() << "Failed to instance";
        return;
    }

    printf("PongGame size: %d obj: %x\n", myObject->m_type->getSize(), myObject->m_obj);

    if (myObject)
    {
        RzVirtualMachine vm(&ctx);
        vm.setContext(&ctx);

        vm.m_mainContext.pushObject(myObject);
        vm.call("PongGame.main");
    }

    printf("Finished successfully\n");
}
