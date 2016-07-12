opencv2{
message("With opencv2")
	win32:{
		  OPENCVDIR = "C:/NV/libs/opencv-2.4/install/w32/vc10/shared"
		  exists($$OPENCVDIR) {
			 CONFIG(debug, debug|release) {
                    CVVER+=245d
					OPENCVDIR=$${OPENCVDIR}/debug
             }
             CONFIG(release, debug|release) {
                    CVVER+=245
					OPENCVDIR=$${OPENCVDIR}/release
             }
			  DEFINES += WIN32_BUILD
			  DEFINES += USEOPENCV2_PORT
			  DEFINES += USEOPENCV2
					
					INCLUDEPATH += $${OPENCVDIR}/include

					win32-msvc* {
						LIBS += -L$${OPENCVDIR}/lib \
                                                -lopencv_core$${CVVER} \
                                                -lopencv_imgproc$${CVVER} \
                                                -lopencv_highgui$${CVVER} \
                                                -lopencv_calib3d$${CVVER} \
                                                -lopencv_ml$${CVVER}
						INCLUDEPATH += $${OPENCVDIR}/3rdparty/include
                                        }
				 message("OpenCV2")
				 DEPENDPATH += $${OPENCVDIR}/bin
				 message("OpenCV2 libraries found in $${OPENCVDIR}")
			}else:error("OpenCV2 libraries not found.")
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
}
