#include <utils/test_framework.h>

void RzUnitTest::setError(ErrorFlag err) {
    m_errorFlags = err;
}

void RzUnitTestSuite::add(RzUnitTest* test) {
    m_tests.push_back(RzTestPtr(test));
}

int RzUnitTestSuite::exec() {
    bool passes = true;

    for (auto& t : m_tests) {
        t->execute();
        if (t->errorFlags() != 0)
            passes = false;
    }
    return passes ? 1 : 0;
}
