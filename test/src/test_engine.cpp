#include "gtest/gtest.h"

#include <razer/frontend/Parser/TokenParser.h>
#include <razer/frontend/Parser/RzParser.h>
#include <razer/runtime/RzEngine.h>
#include <RzSDK/ExportStd.h>

class TestEngine : public ::testing::Test {
protected:

};

class ClassA
{
    int a;
    int b;
    int c;
};

TEST_F(TestEngine, TestPrimitiveCreation) {

    RzEngine engine;
    EXPECT_EQ(engine.getTypeInfo("int32") != nullptr, true);
    EXPECT_EQ(engine.getTypeInfo("void") != nullptr, true);
    EXPECT_EQ(engine.getTypeInfo("bool") != nullptr, true);
    EXPECT_EQ(engine.getTypeInfo("float") != nullptr, true);
    EXPECT_EQ(engine.getTypeInfo("string") != nullptr, true);
    EXPECT_EQ(engine.getTypeInfo("var") != nullptr, true);
}

TEST_F(TestEngine, TestModuleInit) {

    RzEngine engine;
    RegisterStd(&engine);
    EXPECT_EQ(engine.getModule("std") != nullptr, true);
}

