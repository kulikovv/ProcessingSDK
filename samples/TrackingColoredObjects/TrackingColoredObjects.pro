#-------------------------------------------------
#
# Project created by QtCreator 2014-03-25T10:45:27
#
#-------------------------------------------------

TARGET = TrackingColoredObjects
TEMPLATE = lib

DEFINES += TRACKINGCOLOREDOBJECTS_LIBRARY

SOURCES += trackingcoloredobjects.cpp \
    colordetector.cpp \
    kinectplanebinarynode.cpp \
    depthtracker.cpp

HEADERS += trackingcoloredobjects.h\
        TrackingColoredObjects_global.h \
    colordetector.h \
    kinectplanebinarynode.h \
    Track.h \
    depthtracker.h

CONFIG += opencv #make know the config file that we will use opencv
CONFIG += dirs#mechanism to put the result object into bin/realease folder
LEVEL=../

include(../../qptk/config.pri)#include qptk config file
