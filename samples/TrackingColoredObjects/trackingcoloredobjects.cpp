#include "trackingcoloredobjects.h"
#include <QtPlugin>
#include "colordetector.h"
#include "kinectplanebinarynode.h"
#include "depthtracker.h"

TrackingColoredObjects::TrackingColoredObjects()
{
}

QString TrackingColoredObjects::getLibraryName()
{
    return "Color Tracker";
}

void TrackingColoredObjects::registrate(QString filename)
{
    REGISTER(ColorDetector)
    REGISTER(KinectPlaneBinaryNode)
    REGISTER(MapFilter)
    REGISTER(DepthTracker)
}

#if QT_VERSION < 0x050000
   // Q_PLUGIN_METADATA(IID "com.ethostudio.EthoStudio.NodeLibrary/1.0" FILE "qptk.json")
    Q_EXPORT_PLUGIN2(TrackingColoredObjects, TrackingColoredObjects)
#endif

