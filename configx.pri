#Абсолютные пути до библиотек
win32:{
    #gcc compiler (default)
    SDKDIR = "C:/Develop/Cpp/ProcessingSDK/qptk"
    win32-msvc* {
        #Microsoft visual studio compiler
	DEFINES += WIN32_MSVC
    }
}

unix:{
    isEmpty(SDKDIR):SDKDIR = "/host/Develop"
    message("Linux path = $${LIBS}")
}

CONFIG(debug, debug|release) {
      DESTDIR = C:/Develop/fbin/debug
OBJECTS_DIR = debug/obj
MOC_DIR = debug/moc
}
CONFIG(release, debug|release) {
       DESTDIR = C:/Develop/fbin/release
MOC_DIR = release/moc
OBJECTS_DIR = release/obj
}
message("Output dir is $${DESTDIR}")

#messages
message("Processing Framework dir is $${SDKDIR}")
#end messages

exists($$SDKDIR) {
        message("Processing Framework libraries found in $${SDKDIR}")
}else:error("Processing Framework libraries not found.")

# OpenCV - библиотека для обработки ихзображений
CONFIG += opencv3


INCLUDEPATH += $${SDKDIR}

nosdk:{
    message("Compilling without SDK")
}else{
    message("Compilling with SDK")
    CONFIG(debug, debug|release) {
          LIBS += -L$${DESTDIR}
    }
    CONFIG(release, debug|release) {
          LIBS += -L$${DESTDIR}
    }
    LIBS +=-lsdk
}
include(../opencv3.pri)
