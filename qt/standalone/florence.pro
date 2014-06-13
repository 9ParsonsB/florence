CONFIG += qt debug
QT += svg xml widgets x11extras
TARGET = florence
TEMPLATE = app
SOURCES += main.cpp \
    simulator.cpp \
    florenceexception.cpp \
    systemmap.cpp \
    systemmapkey.cpp \
    keysym2ucs.c
HEADERS += \
    simulator.h \
    florenceexception.h \
    systemmap.h \
    systemmapkey.h \
    keysym2ucs.h
INCLUDEPATH += ../src
LIBS += -lflorence_qt -lXtst -lX11
