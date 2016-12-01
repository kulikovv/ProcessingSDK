opencv3{
message("With opencv3")
        OPENCVDIR = "D:/NV/libs/opencv-3.1/build"
	exists($$OPENCVDIR) {
        CONFIG(debug, debug|release) {
                      CVVER+=310d
        }
        CONFIG(release, debug|release) {
                      CVVER+=310
        }
        DEFINES += USEOPENCV2
        INCLUDEPATH += $${OPENCVDIR}/include
        win32-msvc* {
                        LIBS += -L$${OPENCVDIR}/x64/vc14/lib \
                                -lopencv_world$${CVVER}
		}
        message("OpenCV3")
        }else:error("OpenCV 3 libraries not found.")
}
