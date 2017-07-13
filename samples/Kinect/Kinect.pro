TARGET = KinectLibrary
TEMPLATE = lib

# Kinect support integred
# add kinect - to use libfreenect
# add openni - to use openni
# add openni2 - to compile with MS Kinect SDK
# for readonly freenect (*.kin) files use no keys!!!
CONFIG += plugin opencv ffmpeg

DEFINES += KINECTLIBRARY_LIBRARY

SOURCES += kinectlibrary.cpp\
        rgbd2rgb.cpp \
    kinectwriter.cpp \
    kinectreader.cpp \
    rgbdkinect.cpp

HEADERS += kinectlibrary.h\
        kinectlibrary_global.h\
        rgbd2rgb.h \
    kinectwriter.h \
    kinectreader.h \
    rgbdkinect.h

include(C:/Develop/Framework/qptk/config.pri)

freenect:{
    message("Building with Freenect driver")
    HEADERS += KinectSerial.hpp \
                Nodes/kinectnode.h
    SOURCES += Nodes/kinectnode.cpp

    KINECTDIR = "C:/Develop/ethostudio/Plugins/KinectSensor/deps/freenect"
    win32:{
        exists($$KINECTDIR) {
            DEFINES += LIBFREENECT
            INCLUDEPATH += $${KINECTDIR}/include
            win32-msvc* {

            }else{
            LIBS += -L$${KINECTDIR}/lib/gcc \
                  -lfreenect \
                  -lfreenect_sync
            }
            message("Freenect libraries found in $${KINECTDIR}")
        }else:message("Freenect libraries not found.")
    }
    unix:{
        DEFINES += LIBFREENECT
        KINECTDIR = "deps/freenect"
        INCLUDEPATH += $${KINECTDIR}/include
        LIBS += -lfreenect \
        -lfreenect_sync
    }
}

openni:{
    message("Building with OpenNI driver")
    DEFINES += OPENNI
    HEADERS += kinectopenni.h
    SOURCES += kinectopenni.cpp


    win32:{
        OPENIDIR = "C:/Program Files/OpenNI"
        INCLUDEPATH += $${OPENIDIR}/Include
        win32-msvc* {
            LIBS += -L$${OPENIDIR}/Lib\
                                        -lOpenNI
        }else:error("OpenNI is unavalabal for MinGW on Win32")

    }
    unix:{
        INCLUDEPATH += /home/kiuser/Developer/OpenNI/Include
        LIBS +=  -lOpenNI
    }
}

openni2:{
    message("Building with OpenNI2 driver")
    DEFINES += OPENNI2
    HEADERS += kinectopenni2.h
    SOURCES += kinectopenni2.cpp
    DEFINES -= WIN32_MSVC

    win32:{
        OPENIDIR = "C:/Program Files (x86)/OpenNI2"
        INCLUDEPATH += $${OPENIDIR}/Include
        win32-msvc* {
            LIBS += -L$${OPENIDIR}/Lib\
                                        -lOpenNI2
        }else:error("OpenNI2 is unavalabal for MinGW on Win32")

    }
    unix:{
        INCLUDEPATH += /home/kiuser/Developer/OpenNI2/Include
        LIBS +=  -lOpenNI2
    }
}

kinect2sdk:{
    message("Building with Kinect 2 SDK driver")
    DEFINES += KINECT2SDK
    HEADERS += kinect2sdk.h\
               sal.h
    SOURCES += kinect2sdk.cpp

    KINECTSDKDIR = "C:/Program Files/Microsoft SDKs/Kinect/v2.0_1409"
    INCLUDEPATH += $${KINECTSDKDIR}/inc

    LIBS += -L$${KINECTSDKDIR}/Lib/x86\
            -lKinect20
}

win32:{
    openni:{
    }
    freenect:{
        LIBUSBDIR = "deps/libusb-win32" #Директория к библиотеке libusb
        exists($$LIBUSBDIR) {
            INCLUDEPATH += $${LIBUSBDIR}/include
            win32-msvc* {
                LIBS += -L$${LIBUSBDIR}/lib/msvc
            }
            else:{
                LIBS += -L$${LIBUSBDIR}/lib/gcc
            }
            LIBS += -lusb
            message("LibUsb found")
         }else: error("Lib usb not found")
    }
}
unix:{
        LIBS += -lusb-1.0
}

ffmpeg:{
    LIBFFMPEGDIR = "C:/Develop/Cpp/Libs/ffmpeg-1.2/install/w64/gcc47/release"
    INCLUDEPATH += $${LIBFFMPEGDIR}/include
    LIBS += -L$${LIBFFMPEGDIR}/lib/vc10\
            -lavcodec\
            -lavdevice\
            -lavfilter\
            -lavformat\
            -lavutil\
            -lswresample\
            -lswscale
}
include(../../qptk/config.pri)#include qptk config file
include(VideoIO/VideoIO.pri)
