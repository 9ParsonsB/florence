CONFIG += qt debug
QT += svg xml widgets x11extras dbus
TARGET = florence
TEMPLATE = app
SOURCES += main.cpp \
    simulator.cpp \
    florenceexception.cpp \
    systemmap.cpp \
    systemmapkey.cpp \
    keysym2ucs.c \
    service.cpp
HEADERS += \
    simulator.h \
    florenceexception.h \
    systemmap.h \
    systemmapkey.h \
    keysym2ucs.h \
    service.h
INCLUDEPATH += ../src
LIBS += -lflorence_qt -lXtst -lX11
