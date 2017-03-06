
INCLUDEPATH += ../src/ \
               ../include

LIBS += -L../lib -llibrazer

HEADERS += \
    src/aeon_type_utils.h \
    src/testbed.h

SOURCES += \
    src/aeon_type_utils.cpp \
    src/testbed.cpp
