
INCLUDEPATH += ../src/ \
               ../include

LIBS += -L../lib -llibrazer

HEADERS += \
    src/aeon_type_utils.h \
    src/testbed.h \
    src/run_arkanoid.h

SOURCES += \
    src/aeon_type_utils.cpp \
    src/testbed.cpp \
    src/run_arkanoid.cpp

PRE_TARGETDEPS += $$PWD/../lib
PRE_TARGETDEPS += $$PWD/../lib/librazer.lib
