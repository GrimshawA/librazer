#include "parser_test.h"
#include "gtest/gtest.h"

#include <RazerParser/Parser/TokenParser.h>
#include <RazerParser/Parser/RzParser.h>
#include <RazerRuntime/RzEngine.h>

class TestBinding : public ::testing::Test {
protected:

};

class ClassA
{
    int a;
    int b;
    int c;
};

TEST_F(TestBinding, TestRegisterClass) {

    RzEngine engine;
    RzModule* m = engine.createModule("main");

    RzType* t = m->registerType("ClassA", sizeof(ClassA));

    EXPECT_EQ(m->getType("ClassA") != nullptr, true);
    EXPECT_EQ(m->getType("ClassA"), t);

    EXPECT_EQ(t->getSize(), sizeof(ClassA));
    EXPECT_EQ(t->getName(), std::string("ClassA"));

    // Duplicated type not allowed
    EXPECT_EQ(m->registerType("ClassA", sizeof(ClassA)), nullptr);
}

