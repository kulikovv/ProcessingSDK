opencv3{
message("With opencv3")
        OPENCVDIR = "C:/NV/libs/opencv-3.0/opencv/build"
	exists($$OPENCVDIR) {
        CONFIG(debug, debug|release) {
                      CVVER+=300d
        }
        CONFIG(release, debug|release) {
                      CVVER+=300
        }
        DEFINES += USEOPENCV2
        INCLUDEPATH += $${OPENCVDIR}/include
        win32-msvc* {
			LIBS += -L$${OPENCVDIR}/x86/vc10/lib \
                                -lopencv_world$${CVVER}
		}
        message("OpenCV3")
        }else:error("OpenCV 3 libraries not found.")
}
