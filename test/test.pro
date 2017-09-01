INCLUDEPATH += ../src/

CONFIG += c++11

INCLUDEPATH += googletest/include \
                googletest

LIBS += -L$$PWD/../lib -llibrazer

PRE_TARGETDEPS += $$PWD/../lib
PRE_TARGETDEPS += $$PWD/../lib/librazer.lib

DEPENDPATH += $$PWD/../lib
PRE_TARGETDEPS += $$PWD/../lib
$${TARGET}.depends += $$PWD/../lib

# SDL
LIBS += -L$$PWD/../3rdparty/lib/x86 -lSDL2


HEADERS += \
    vm_test/type_registration_test.h \
    vm_test/value_test.h \
    parser_test/parser_test.h \
    googletest/src/gtest-internal-inl.h \
    googletest/include/gtest/gtest.h \
    googletest/include/gtest/gtest_pred_impl.h \
    googletest/include/gtest/gtest_prod.h \
    googletest/include/gtest/gtest-death-test.h \
    googletest/include/gtest/gtest-message.h \
    googletest/include/gtest/gtest-param-test.h \
    googletest/include/gtest/gtest-param-test.h.pump \
    googletest/include/gtest/gtest-printers.h \
    googletest/include/gtest/gtest-spi.h \
    googletest/include/gtest/gtest-test-part.h \
    googletest/include/gtest/gtest-typed-test.h \
    googletest/include/gtest/internal/gtest-death-test-internal.h \
    googletest/include/gtest/internal/gtest-filepath.h \
    googletest/include/gtest/internal/gtest-internal.h \
    googletest/include/gtest/internal/gtest-linked_ptr.h \
    googletest/include/gtest/internal/gtest-param-util.h \
    googletest/include/gtest/internal/gtest-param-util-generated.h \
    googletest/include/gtest/internal/gtest-port.h \
    googletest/include/gtest/internal/gtest-port-arch.h \
    googletest/include/gtest/internal/gtest-string.h \
    googletest/include/gtest/internal/gtest-tuple.h \
    googletest/include/gtest/internal/gtest-tuple.h.pump \
    googletest/include/gtest/internal/gtest-type-util.h \
    googletest/include/gtest/internal/custom/gtest.h \
    googletest/include/gtest/internal/custom/gtest-port.h \
    googletest/include/gtest/internal/custom/gtest-printers.h

SOURCES += \
    vm_test/type_registration_test.cpp \
    vm_test/value_test.cpp \
    parser_test/parser_test.cpp \
    googletest/src/gtest.cc \
    googletest/src/gtest_main.cc \
    googletest/src/gtest-all.cc \
    googletest/src/gtest-death-test.cc \
    googletest/src/gtest-filepath.cc \
    googletest/src/gtest-port.cc \
    googletest/src/gtest-printers.cc \
    googletest/src/gtest-test-part.cc \
    googletest/src/gtest-typed-test.cc \
    test_calls.cpp \
    test_binding.cpp \
    test_engine.cpp
