
INCLUDEPATH += ../src/ \
               ../include

LIBS += -L$$PWD/../lib -llibrazer

HEADERS += \
    src/aeon_type_utils.h \
    src/testbed.h \
    src/run_arkanoid.h \
    src/run_pong.h

SOURCES += \
    src/aeon_type_utils.cpp \
    src/testbed.cpp \
    src/run_arkanoid.cpp \
    src/run_pong.cpp

PRE_TARGETDEPS += $$PWD/../lib
PRE_TARGETDEPS += $$PWD/../lib/librazer.lib

DEPENDPATH += $$PWD/../lib
PRE_TARGETDEPS += $$PWD/../lib
$${TARGET}.depends += $$PWD/../lib

# SDL
LIBS += -L$$PWD/../3rdparty/lib/x86 -lSDL2 -lSDL2main
