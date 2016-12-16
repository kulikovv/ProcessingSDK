VIDEO_IO_CONFIG *= shared use_headers use_libs
OPENCV_CONFIG *= shared use_headers use_libs

include(../_VideoIO.pri)
include($${NV_PRI_DIR}/libs/_opencv.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

DEPENDPATH *= "$${VIDEO_IO_INCLUDE_DIR}"

LIBS += $$VIDEO_IO_LIBS
LIBS += $$opencv_link(highgui imgproc core)
LIBS += $$NV_COMMON_LIBS

SOURCES += \
    ../../probe/main.cpp
