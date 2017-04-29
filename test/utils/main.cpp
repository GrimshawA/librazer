/*
 * Unit Test Suite
 * */

#include <Logger.h>
#include <utils/test_framework.h>

#include <parser_test/parser_test.h>

int main(int argc, char** argv) {
    RzUnitTestSuite suite;
    suite.add(new RzTestParser);
    return suite.exec();
}
