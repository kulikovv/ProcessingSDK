opencv3{
message("With opencv3")
        OPENCVDIR = "C:/Develop/Cpp/Libs/opencv/build"
	exists($$OPENCVDIR) {
        CONFIG(debug, debug|release) {
                      CVVER+=320d
        }
        CONFIG(release, debug|release) {
                      CVVER+=320
        }
        DEFINES += USEOPENCV2
        INCLUDEPATH += $${OPENCVDIR}/include
        win32-msvc* {
                    !contains(QMAKE_TARGET.arch, x86_64) {
                            message("x86 build")

                            ## Windows x86 (32bit) specific build here
                            LIBS += -L$${OPENCVDIR}/x86/vc14/lib \
                                -lopencv_world$${CVVER}

                        } else {
                            message("x86_64 build")

                            ## Windows x64 (64bit) specific build here
                            LIBS += -L$${OPENCVDIR}/x64/vc14/lib \
                                -lopencv_world$${CVVER}
                        }

		}
        message("OpenCV3")
        }else:error("OpenCV 3 libraries not found.")
}
