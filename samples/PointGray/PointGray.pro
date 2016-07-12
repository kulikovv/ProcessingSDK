#-------------------------------------------------
#
# Project created by QtCreator 2013-02-05T19:57:34
#
#-------------------------------------------------


TARGET = PointGray
TEMPLATE = lib

DEFINES += POINTGRAY_LIBRARY

SOURCES += pointgray.cpp\
           pointgraycamera.cpp \
    pointgraysynchcameras.cpp

HEADERS += pointgray.h\
        PointGray_global.h\
        pointgraycamera.h \
    pointgraysynchcameras.h
CONFIG += opencv


unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32:{
QMAKE_CXXFLAGS+= -openmp
QMAKE_LFLAGS +=  -openmp
INCLUDEPATH += "C:/Program Files (x86)/Point Grey Research/FlyCapture2/include"
LIBS += -L"C:/Program Files (x86)/Point Grey Research/FlyCapture2/lib"\
        -lFlyCapture2

}
include(../../qptk/config.pri)
