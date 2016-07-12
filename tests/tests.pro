#-------------------------------------------------
#
# Project created by QtCreator 2013-03-04T15:57:39
#
#-------------------------------------------------
QT += testlib gui


TARGET = tests
CONFIG   += console
CONFIG   += dirs

LEVEL=""

TEMPLATE = app


SOURCES += main.cpp \
    sdk/io/text/textiotest.cpp \
    sdk/base/espobjecttest.cpp \
    sdk/base/nodetest.cpp \
    sdk/base/graphtest.cpp \
    sdk/io/media/videooutputtest.cpp \
    sdk/utils/graphdomtest.cpp \
    sdk/types/labeleddatatest.cpp \
    sdk/types/pdatatest.cpp

CONFIG += opencv
include (../qptk/config.pri)

HEADERS += \
    AutoTest.h \
    StringTest.h \
    sdk/io/text/textiotest.h \
    sdk/base/espobjecttest.h \
    sdk/base/nodetest.h \
    sdk/base/graphtest.h \
    sdk/io/media/videooutputtest.h \
    TestObjects.h \
    sdk/utils/graphdomtest.h \
    sdk/types/labeleddatatest.h \
    sdk/types/pdatatest.h
