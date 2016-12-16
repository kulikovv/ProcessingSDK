VIDEO_IO_CONFIG *= shared use_headers

FFMPEG_CONFIG *= shared use_headers
OPENCV_CONFIG *= shared use_headers

contains(VIDEO_IO_CONFIG, shared) {
    FFMPEG_CONFIG *= use_libs
    OPENCV_CONFIG *= use_libs
}

include(../_VideoIO.pri)

TEMPLATE = lib
CONFIG += $$VIDEO_IO_LINK_CFG
CONFIG -= qt

DEFINES *= $$NV_WINVER_DEFINE
DESTDIR = $${VIDEO_IO_INSTALL_DIR}/lib
DLLDESTDIR = $${VIDEO_IO_INSTALL_DIR}/bin

isEqual(VIDEO_IO_LINK_CFG, static) {
    *-msvc*: QMAKE_CXXFLAGS_DEBUG *= /Fd"$${DESTDIR}/$${TARGET}.pdb"
}

isEqual(VIDEO_IO_LINK_CFG, shared) {
    DEFINES *= VIDEO_IO_EXPORTS
    *-msvc*: QMAKE_LFLAGS_DEBUG *= /PDB:"$${DLLDESTDIR}/$${TARGET}.pdb"
    LIBS += $$ffmpeg_link(swscale avformat avcodec avutil)
    LIBS += $$opencv_link(core)
    LIBS += $$NV_COMMON_LIBS
}

HEADERS += \
    ../../VideoIO/VideoWriter.h \
    ../../VideoIO/VideoReader.h \
    ../../VideoIO/VideoIO.h \
    ../../VideoIO/FFMpeg.h \
    ../../VideoIO/Errors.h \
    ../../VideoIO/Defs.h \
    ../../VideoIO/AnnoyingWarningsOn.h \
    ../../VideoIO/AnnoyingWarningsOff.h \
    ../../VideoIO/Utils.h \
    ../../VideoIO/UtilsInternal.h

SOURCES += \
    ../../VideoIO/VideoWriter.cpp \
    ../../VideoIO/VideoReader.cpp \
    ../../VideoIO/FFMpeg.cpp \
    ../../VideoIO/Errors.cpp \
    ../../VideoIO/Utils.cpp
