#Абсолютные пути до библиотек
win32:{
    #gcc compiler (default)
    SDKDIR = "C:/Develop/Framework/qptk/"

    OPENCVDIR = "D:\OpenCV\build_gcc"

    win32-msvc* {
        #Microsoft visual studio compiler
        SDKDIR = "C:/Develop/Framework/qptk/"
	DEFINES += WIN32_MSVC
        OPENCVDIR = "D:/OpenCV/build_msvc"
    }

}
unix:{
    isEmpty(SDKDIR):SDKDIR = "/host/Develop"
    message("Linux path = $${LIBS}")
}

CONFIG(debug, debug|release) {
      DESTDIR = $$OUT_PWD/../$${LEVEL}bin/debug
}
CONFIG(release, debug|release) {
       DESTDIR = $$OUT_PWD/../$${LEVEL}bin/release
}
message("Output dir is $${DESTDIR}")

#messages
message("Processing Framework dir is $${SDKDIR}")
message("Opencv dir is $${OPENCVDIR}")
#end messages

exists($$SDKDIR) {
        message("Processing Framework libraries found in $${SDKDIR}")
}else:error("Processing Framework libraries not found.")

# OpenCV - библиотека для обработки ихзображений
# (Если возникнет необходимость его достаточно объявить в
# параметрах $$QWTDIR)
CONFIG += opencv2

MOC_DIR = moc
OBJECTS_DIR = obj
INCLUDEPATH += $${SDKDIR}
nosdk:{
    message("Compilling without SDK")
}else{
    message("Compilling with SDK")
    CONFIG(debug, debug|release) {
          LIBS += -L C:/Develop/framework-build-desktop/bin/debug
    }
    CONFIG(release, debug|release) {
           LIBS += -L C:/Develop/framework-build-desktop/bin/release
    }
    LIBS +=-lsdk
}
win32:{
    DEFINES += WIN32_BUILD
    opencv{

        exists($$OPENCVDIR) {
        opencv1{
            DEFINES += USEOPENCV1
            INCLUDEPATH += $${OPENCVDIR}/cv/include \
                $${OPENCVDIR}/cxcore/include \
                $${OPENCVDIR}/otherlibs/highgui
            LIBS += -L$${OPENCVDIR}/lib \
                -lcv \
                -lcxcore \
                -lhighgui
            message("OpenCV1")
         }
         opencv2{
            DEFINES += USEOPENCV2_PORT
            DEFINES += USEOPENCV2
            INCLUDEPATH += $${OPENCVDIR}/include

            win32-msvc* {
                LIBS += -L$${OPENCVDIR}/lib \
                -lopencv_core220 \
                -lopencv_imgproc220 \
                -lopencv_highgui220 \
                -lopencv_calib3d220 \
                -lopencv_ml220
                INCLUDEPATH += $${OPENCVDIR}/3rdparty/include

                LIBS += -L$${OPENCVDIR}/3rdparty/lib\
                                -lvideoInput
            }else{
                LIBS += -L$${OPENCVDIR}/lib \
                -lopencv_core220.dll \
                -lopencv_imgproc220.dll \
                -lopencv_highgui220.dll \
                -lopencv_calib3d220.dll\
                -lopencv_ml220.dll
            }


            message("OpenCV2")
         }
         DEPENDPATH += $${OPENCVDIR}/bin
         message("OpenCV libraries found in $${OPENCVDIR}")
        }else:error("OpenCV libraries not found.")
    }
}
unix:{
    DEFINES += UNIX_BUILD
    opencv{
        opencv1{
            DEFINES += USEOPENCV2_PORT
        }
        opencv2{
            DEFINES += USEOPENCV2
        }
        INCLUDEPATH += /usr/local/include
        LIBS += -lopencv_core \
            -lopencv_legacy \
            -lopencv_highgui
        message("OpenCV libraries connected")
    }
}
