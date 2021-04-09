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
    service.cpp \
    settingsservice.cpp \
    manager.cpp \
    settingsdialog.cpp
HEADERS += \
    simulator.h \
    florenceexception.h \
    systemmap.h \
    systemmapkey.h \
    keysym2ucs.h \
    service.h \
    settingservice.h \
    manager.h \
    settingsdialog.h
INCLUDEPATH += ../src
LIBS += -lflorence_qt -lXtst -lX11

prefix.path = /usr
target.path = $$prefix.path/bin
INSTALLS += target

FORMS += \
    config.ui
