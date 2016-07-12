#-------------------------------------------------
#
# Project created by QtCreator 2013-07-22T11:25:57
#
#-------------------------------------------------

TARGET = TrackingTest
TEMPLATE = lib

DEFINES += TRACKINGTEST_LIBRARY

CONFIG += opencv

SOURCES += trackingtest.cpp \
    datagenerator.cpp \
    imagegenerator.cpp \
    object3dgenerator.cpp

HEADERS += trackingtest.h \
    imagegenerator.h \
    datagenerator.h \
    trackingtest_global.h \
    object3dgenerator.h

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

include(../../qptk/config.pri)
