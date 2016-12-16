# VIDEO_IO_CONFIG options
#     static | shared
#     use_headers
#     use_libs
#     use_dlls (устанавливается скриптом, если VIDEO_IO_CONFIG содержит shared и use_libs)
#
# video_io_link modules
#     VideoIO

include($${NV_PRI_DIR}/libs/_common.pri)

VIDEO_IO_DIR = $$PWD/..

contains(VIDEO_IO_CONFIG, static) | contains(VIDEO_IO_CONFIG, shared) {
    contains(VIDEO_IO_CONFIG, static): contains(VIDEO_IO_CONFIG, shared) {
        error(incompatible VIDEO_IO_CONFIG options: static, shared)
    }

    contains(VIDEO_IO_CONFIG, static): VIDEO_IO_LINK_CFG = static
    contains(VIDEO_IO_CONFIG, shared): VIDEO_IO_LINK_CFG = shared

    VIDEO_IO_INSTALL_DIR = $$install_dir($${VIDEO_IO_DIR}, $${VIDEO_IO_LINK_CFG})

    contains(VIDEO_IO_CONFIG, use_headers) {
        VIDEO_IO_INCLUDE_DIR = $${VIDEO_IO_DIR}
        INCLUDEPATH *= "$${VIDEO_IO_INCLUDE_DIR}"
        isEqual(VIDEO_IO_LINK_CFG, static): DEFINES *= VIDEO_IO_STATIC
        OPENCV_CONFIG *= $$VIDEO_IO_LINK_CFG use_headers
    }

    contains(VIDEO_IO_CONFIG, use_libs) {
        VIDEO_IO_LIB_DIR = $${VIDEO_IO_INSTALL_DIR}/lib

        LIBS *= -L"$${VIDEO_IO_LIB_DIR}"
        isEqual(VIDEO_IO_LINK_CFG, static) {
            FFMPEG_CONFIG *= shared use_libs
            OPENCV_CONFIG *= $$VIDEO_IO_LINK_CFG use_libs
        }
        isEqual(VIDEO_IO_LINK_CFG, shared) {
            VIDEO_IO_CONFIG *= use_dlls
        }

        defineReplace(video_io_link) {
            RESULT =
            for(module, 1): RESULT += -l$${module}
            return($$RESULT)
        }
    }
}

contains(VIDEO_IO_CONFIG, use_dlls) {
    VIDEO_IO_DLLS_DIR = $$install_dir($${VIDEO_IO_DIR}, shared)/bin
    LIBS *= -L"$${VIDEO_IO_DLLS_DIR}"
    FFMPEG_CONFIG *= use_dlls
    OPENCV_CONFIG *= use_dlls
}

include($${NV_PRI_DIR}/libs/_ffmpeg.pri)
include($${NV_PRI_DIR}/libs/_opencv.pri)

contains(VIDEO_IO_CONFIG, static) | contains(VIDEO_IO_CONFIG, shared) {
    contains(VIDEO_IO_CONFIG, use_libs) {
        VIDEO_IO_LIBS = $$video_io_link(VideoIO)
        isEqual(VIDEO_IO_LINK_CFG, static) {
            VIDEO_IO_LIBS += $$ffmpeg_link(swscale avformat avcodec avutil)
            VIDEO_IO_LIBS += $$opencv_link(core)
        }
    }
}
