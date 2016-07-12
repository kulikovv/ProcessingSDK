#ifndef POINTGRAY_H
#define POINTGRAY_H

#include "PointGray_global.h"
#include <interfaces.h>

class POINTGRAYSHARED_EXPORT PointGray:public QObject,NodeLibrary{
    Q_OBJECT
    Q_INTERFACES(NodeLibrary)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "com.ethostudio.EthoStudio.PointGray/1.0" FILE "PointGray.json")
#endif
public:
    PointGray();
    virtual QString getLibraryName();
    virtual void registrate(QString filename);

};

#endif // POINTGRAY_H
