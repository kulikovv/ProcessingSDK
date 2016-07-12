#ifndef PROCESSINGFRAMEWORK_SDK_H
#define PROCESSINGFRAMEWORK_SDK_H

#include <QObject>
#include <interfaces.h>


class Sdk : public QObject, public NodeLibrary
{
    Q_OBJECT
    Q_INTERFACES(NodeLibrary)
#if QT_VERSION > 0x050000
    Q_PLUGIN_METADATA(IID "com.ethostudio.EthoStudio.NodeLibrary/1.0" FILE "qptk.json")
#endif
public:
    explicit Sdk(QObject *parent = 0);
	
    virtual QString getLibraryName();
    virtual void registrate(QString filename);

};

#endif // SDK_H
