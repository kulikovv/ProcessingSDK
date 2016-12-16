#ifndef KINECTLIBRARY_H
#define KINECTLIBRARY_H

#include "kinectlibrary_global.h"

#include <QObject>
#include <interfaces.h>

class KINECTLIBRARYSHARED_EXPORT KinectLibrary : public QObject,NodeLibrary
{
    Q_OBJECT
    Q_INTERFACES(NodeLibrary)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "com.ethostudio.EthoStudio.KinectLibrary/1.0" FILE "KinectLibrary.json")
#endif
public:
    KinectLibrary();
    virtual QString getLibraryName();
    virtual void registrate(QString filename);
};

#endif // KINECTLIBRARY_H

