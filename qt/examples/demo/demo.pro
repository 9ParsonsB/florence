QT += svg xml widgets
TARGET = demo
TEMPLATE = app
SOURCES += main.cpp mainwindow.cpp lineedit.cpp
HEADERS += mainwindow.h lineedit.h
FORMS += mainwindow.ui
INCLUDEPATH += ../../src
LIBS += -lflorence_qt
