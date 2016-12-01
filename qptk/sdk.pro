#-------------------------------------------------
#
# Project created by QtCreator 2011-10-13T13:17:36
#
#-------------------------------------------------

QT       += core gui network xml opengl

greaterThan(QT_MAJOR_VERSION,4){
    QT += widgets
}

TARGET = sdk
TEMPLATE = lib

DEFINES += SDK_LIBRARY



CONFIG += opencv nosdk dirs

include(config.pri)

SOURCES += sdk.cpp \
    base/node.cpp \
    base/esp.cpp \
    base/graph.cpp \
    io/sourcenode.cpp \
    io/outputnode.cpp \
    process/processnode.cpp \
    synch/synchfactory.cpp \
    synch/synch.cpp \
    types/rgb.cpp \
    types/event.cpp \
    camera/steriocalibration.cpp \
    camera/calibration.cpp \
    io/media/imagenode.cpp \
    io/media/videosource.cpp \
    io/media/videooutput.cpp \
    types/rgbd.cpp \
    io/kinect/kinectstorage.cpp \
    io/kinect/kinectplaynode.cpp \
    io/kinect/kinectoutnode.cpp \
    synch/leadersynch.cpp \
    visual/visualnode.cpp \
    visual/imagevisualnode.cpp \
    visual/imageview.cpp \
    visual/forms/simpleimageview.cpp \
    visual/forms/calibrationdialog.cpp \
    io/capture/stereocameranode.cpp \
    io/capture/singlecameranode.cpp \
    io/capture/opencvcameranode.cpp \
    io/capture/cameranode.cpp \
    process/resizenode.cpp \
    process/binarynode.cpp \
    utils/uiautofitter.cpp \
    utils/graphxml.cpp \
    io/text/datasource.cpp \
    io/text/dataoutput.cpp \
    visual/kinect/kinectrgb.cpp \
    visual/kinect/kinectdepth.cpp \
    visual/text/tableview.cpp \
    visual/zoomimageview.cpp \
    visual/forms/zoomwidget.cpp \
    workflow/parallel.cpp \
    workflow/adapter.cpp \
    workflow/workflownode.cpp \
    utils/singleapplication.cpp \
    process/negative.cpp \
    utils/graphparser.cpp \
    utils/graphsettings.cpp \
    visual/forms/layoutwidget.cpp \
    visual/forms/layoututils.cpp \
    visual/layoutnode.cpp \
    utils/logs.cpp \
    io/capture/ipcamera.cpp \
    io/inoutnode.cpp \
    io/text/dataionode.cpp \
    visual/forms/holderdialog.cpp \
    visual/forms/qtablewidgetcopy.cpp \
    types/data.cpp \
    network/tcpabstractsender.cpp \
    camera/calibrationnode.cpp \
    process/fpslistener.cpp \
    io/text/streamoutput.cpp \
    io/baseionode.cpp \
    #visual/ogl/glview.cpp \
    #visual/oglview.cpp \
    #visual/ogl/glcloudview.cpp \
    utils/graphdom.cpp \
    io/capture/videoinputcapture.cpp \
    types/callbackhook.cpp \
  #  io/text/multiobjectdata.cpp \
    types/multimap.cpp \
    utils/rawdataexport.cpp \
    utils/opencvexport.cpp \
    utils/matlabexport.cpp
 #   visual/text/multiobjecttableview.cpp

HEADERS += sdk.h\
        sdk_global.h \
    base/node.h \
    base/esp.h \
    base/dyncreation.h \
    base/dynconvert.h \
    interfaces.h \
    base/graph.h \
    io/sourcenode.h \
    io/outputnode.h \
    process/processnode.h \
    synch/synchfactory.h \
    synch/synch.h \
    types/rgb.h \
    types/serializable.h \
    types/event.h \
    camera/steriocalibration.h \
    camera/icalibration.h \
    camera/calibration.h \
    io/media/imagenode.h \
    io/media/videosource.h \
    io/media/videooutput.h \
    types/rgbd.h \
    io/kinect/kinectstorage.h \
    io/kinect/kinectplaynode.h \
    io/kinect/kinectoutnode.h \
    synch/leadersynch.h \
    visual/visualnode.h \
    visual/imagevisualnode.h \
    visual/imageview.h \
    visual/forms/simpleimageview.h \
    visual/forms/calibrationdialog.h \
    io/capture/stereocameranode.h \
    io/capture/singlecameranode.h \
    io/capture/opencvcameranode.h \
    io/capture/cameranode.h \
    process/resizenode.h \
    process/binarynode.h \
    utils/uiautofitter.h \
    utils/graphxml.h \
    io/text/datasource.h \
    types/data.h \
    io/text/dataoutput.h \
    visual/kinect/kinectrgb.h \
    visual/kinect/kinectdepth.h \
    visual/text/tableview.h \
    visual/zoomimageview.h \
    visual/forms/zoomwidget.h \
    workflow/parallel.h \
    workflow/adapter.h \
    workflow/workflownode.h \
    utils/singleapplication.h \
    process/negative.h \
    utils/graphparser.h \
    utils/graphsettings.h \
    visual/forms/layoutwidget.h \
    visual/forms/layoututils.h \
    visual/layoutnode.h \
    utils/logs.h \
    io/capture/ipcamera.h \
    io/inoutnode.h \
    io/text/dataionode.h \
    visual/forms/holderdialog.h \
    visual/forms/qtablewidgetcopy.h \
    network/tcpabstractsender.h \
    camera/calibrationnode.h \
    process/fpslistener.h \
    types/controls.h \
    io/text/streamoutput.h \
    io/baseionode.h \
    types/general_tracking.h \
    #visual/ogl/glview.h \
    #visual/oglview.h \
    #visual/ogl/glcloudview.h \
    utils/graphdom.h \
    io/capture/videoinputcapture.h \
    types/callbackhook.h \
  #  visual/text/multiobjecttableview.h \
 #   io/text/multiobjectdata.h \
    types/multimap.h \
    utils/rawdataexport.h \
    utils/opencvexport.h \
    utils/matlabexport.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE39F43EB
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = sdk.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}

#win32:{
#    QMAKE_CXXFLAGS+= -openmp
#    QMAKE_LFLAGS +=  -openmp
#}
