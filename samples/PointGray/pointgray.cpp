#include "pointgray.h"
#include <QtPlugin>
#include "pointgraycamera.h"
#include "pointgraysynchcameras.h"

PointGray::PointGray()
{
}
QString PointGray::getLibraryName()
{
    return "PointGray";
}

void PointGray::registrate(QString filename)
{
    REGISTER(PointGrayCamera)
    REGISTER(PointGraySynchCameras)
}
#if QT_VERSION < 0x050000
   // Q_PLUGIN_METADATA(IID "com.ethostudio.EthoStudio.NodeLibrary/1.0" FILE "qptk.json")
    Q_EXPORT_PLUGIN2(PointGray,PointGray)
#endif

