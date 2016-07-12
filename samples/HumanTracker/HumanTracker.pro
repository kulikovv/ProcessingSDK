#-------------------------------------------------
#
# Project created by QtCreator 2014-03-26T16:43:29
#
#-------------------------------------------------

TARGET = HumanTracker
TEMPLATE = lib

DEFINES += HUMANTRACKER_LIBRARY

SOURCES += humantracker.cpp \
    humantrackerimpl.cpp

HEADERS += humantracker.h\
        HumanTracker_global.h \
    humantrackerimpl.h

CONFIG += opencv
CONFIG += dirs#mechanism to put the result object into bin/realease folder
LEVEL=../

include(../../qptk/config.pri)#include qptk config file

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

RESOURCES += \
    Resource.qrc
