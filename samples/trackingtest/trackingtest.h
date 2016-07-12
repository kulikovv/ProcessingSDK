#ifndef TESTDATA_H
#define TESTDATA_H

#include "trackingtest_global.h"
#include <interfaces.h>

class TRACKINGTESTSHARED_EXPORT TrackingTest : public QObject, public NodeLibrary
{
    Q_OBJECT
    Q_INTERFACES(NodeLibrary)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "com.ethostudio.EthoStudio.NodeLibrary/1.0" FILE mystyleplugin.json)
#endif
public:
    TrackingTest();

    virtual QString getLibraryName();
    virtual void registrate(QString filename);

};

#endif // TESTDATA_H
