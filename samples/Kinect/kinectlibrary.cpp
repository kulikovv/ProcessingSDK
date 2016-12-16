#include "kinectlibrary.h"
#include <QtPlugin>
#ifdef LIBFREENECT
#include "kinectnode.h"
#endif
#ifdef OPENNI
#include "kinectopenni.h"
#endif
#ifdef OPENNI2
#include "kinectopenni2.h"
#endif
#ifdef KINECT2SDK
#include "kinect2sdk.h"
#endif
#include "Rgbd2Rgb.h"
#include "kinectwriter.h"
#include "kinectreader.h"
//Include here all nodes headers

KinectLibrary::KinectLibrary()
{

}

QString KinectLibrary::getLibraryName()
{
    return "KinectLibrary";
}

void KinectLibrary::registrate(QString filename)
{
    //TODO: register nodes to export from your library
#ifdef LIBFREENECT
    REGISTER(KinectNode)
#endif
#ifdef OPENNI
    REGISTER(KinectOpenNI)
#endif
#ifdef OPENNI2
    REGISTER(KinectOpenNI)
#endif
#ifdef KINECT2SDK
    REGISTER(Kinect2SDK)
#endif
    REGISTER(Rgbd2Rgb)
    REGISTER(KinectWriter)
    REGISTER(KinectReader)
}

#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(KinectLibrary,KinectLibrary)
#endif

