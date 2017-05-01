INCLUDEPATH += ../src/

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
    utils/test_framework.h \
    parser_test/parser_test.h

SOURCES += \
    vm_test/type_registration_test.cpp \
    vm_test/value_test.cpp \
    utils/main.cpp \
    parser_test/parser_test.cpp \
    utils/test_framework.cpp
