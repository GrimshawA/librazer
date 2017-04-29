#include <utils/test_framework.h>

void RzUnitTest::setError(ErrorFlag err) {
    m_errorFlag = err;
}

void RzUnitTestSuite::add(RzUnitTest* test) {
    m_tests.push_back(RzTestPtr(test));
}

int RzUnitTestSuite::exec() {
    bool passes = true;

    for (auto& t : m_tests) {
        t->execute();
        if (t->m_errorFlag != 0)
            passes = false;
    }
    return passes ? 1 : 0;
}
