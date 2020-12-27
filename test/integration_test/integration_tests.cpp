#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include <razer/frontend/RzBuilder.h>
#include <razer/runtime/RzEngine.h>
#include <razer/vm/VirtualMachine.h>
#include <razer/vm/vm_api.h>

struct RzEnv
{
    RzEnv()
    {

    }

    void RunFile(const std::string& file)
    {
        RzBuilder::Batch b;
        b.files.push_back(file);


        RzBuilder builder(ctx);
        bool result = builder.build(b);
        if (!result) {
            std::cout << "Build failed" << std::endl;
            return;
        }

        //ctx.getModule("main")->dumpToFile("code.dmp");
        ctx.getModule("test")->dumpToFile("codeir.dmp");

        AEObject* myObject = ctx.createObject("App");
        if (!myObject)
        {
            std::cout << "Failed to find main" << std::endl;
            return;
        }

        if (myObject)
        {
            RzVirtualMachine vm(ctx);

            auto* cx = &vm.m_mainContext;

            RzStackValue v;
            v.ptr = myObject->m_obj;
            cx->push_value(v);
            //vm.m_mainContext.pushObject(myObject);


            auto* fun = ctx.getModule("test")->getFunction("App.main");

            RzCall(cx, fun);

            std::cout << "main() was executed" << std::endl;
        }
    }

    RzEngine ctx;
    RzModule* stdModule = ctx.createModule("std");
};


int NativeFunc1Val = 0;

void NativeFunc1(RzGeneric g)
{
    /*RzString* a = (RzString*)g.popObject();
    RzString* b = (RzString*)g.popObject();
    RzString* result = new RzString();
    result->set(a->toStdString() + b->toStdString());
    g.pushObject(result);*/

    int val = g.popInt32();

    NativeFunc1Val = val;
}

TEST_CASE( "Case 1", "[integration]" ) {

    RzEnv vm;
    vm.stdModule->registerFunction("void NativeFunc1(int)", NativeFunc1);
    vm.RunFile("int1.rz");

    REQUIRE(NativeFunc1Val == 42);
}

TEST_CASE( "Case 2", "[integration]" ) {


}

