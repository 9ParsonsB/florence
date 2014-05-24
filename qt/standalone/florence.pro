QT += svg xml widgets x11extras
TARGET = florence
TEMPLATE = app
SOURCES += main.cpp \
    simulator.cpp
HEADERS += \
    simulator.h
INCLUDEPATH += ../src
LIBS += -lflorence_qt -lXtst
