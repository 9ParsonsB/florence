QT += svg xml widgets
TARGET = helloworld
TEMPLATE = app
SOURCES += main.cpp lineedit.cpp
HEADERS += lineedit.h
INCLUDEPATH += ../../src
LIBS += -lflorence_qt
