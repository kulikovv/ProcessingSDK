#include "trackingtest.h"
#include "datagenerator.h"
#include "imagegenerator.h"
#include "object3dgenerator.h"
#include <QtPlugin>

TrackingTest::TrackingTest()
{
}

QString TrackingTest::getLibraryName()
{
    return "TrackingTest";
}

void TrackingTest::registrate(QString filename)
{
    REGISTER(DataGenerator)
    REGISTER(ImageGenerator)
    REGISTER(Object3dGenerator)
}
#if QT_VERSION < 0x050000
    Q_EXPORT_PLUGIN2(TrackingTest, TrackingTest)
#endif

