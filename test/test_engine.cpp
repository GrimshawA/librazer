#include "gtest/gtest.h"

#include <RazerParser/Parser/TokenParser.h>
#include <RazerParser/Parser/RzParser.h>
#include <RazerRuntime/RzEngine.h>

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
    engine.init_all();

    EXPECT_EQ(engine.getModule("std") != nullptr, true);
}

