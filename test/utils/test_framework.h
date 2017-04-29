#ifndef RZTESTFRAMEWORK_H__
#define RZTESTFRAMEWORK_H__

#include <vector>
#include <memory>

class RzUnitTestSuite;

class RzUnitTest {
public:
    virtual void execute() = 0;

protected:
    enum ErrorFlag {
        ASSERT_FAILED = 1
    };

    void setError(ErrorFlag err);

private:
    int m_errorFlags = 0;
};

class RzUnitTestSuite {
public:
    void add(RzUnitTest* test);

    int exec();

public:

    typedef std::unique_ptr<RzUnitTest> RzTestPtr;

    std::vector<RzTestPtr> m_tests;
};

#include <cassert>
#define RZTASSERT(x) assert(x)

#endif
