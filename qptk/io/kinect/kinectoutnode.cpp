#include "kinectoutnode.h"
#include <io/kinect/kinectstorage.h>
#include <types/rgbd.h>
#include <utils/logs.h>
#include <QFile>
#include <QFileInfo>
using namespace io;
using namespace io::kinect;
using namespace types;
using namespace base;

KinectOutNode::KinectOutNode(QObject *parent) :
    OutputNode(parent)
{
    //TODO: Initiation source
    _extension = "kin";
    ks=0;
    setEnable(true);
}
KinectOutNode::~KinectOutNode()
{
    //qDebug()<<"KinectOutNode deleted";
    if(0!=ks)
    {
        delete ks;
        ks = 0;
    }
}

//This function is called if this node is enable
void KinectOutNode::receiveProcess(esp data)
{
    if(!ks)
    {
        return;
    }
    QSharedPointer<types::Rgbd> kf = data.getReadOnly<types::Rgbd>();
    if(!kf.isNull())
    {
        ks->write(kf.data());
    }else{
        INFO("KinectOutNode frame is empty")
    }

}

void  KinectOutNode::setUrl(QString value)
{
    if(0!=ks)
    {
        delete ks;
        ks = 0;
    }
    _url = value;
    ks = new KinectStorage(value.toStdString(),KinectStorage::WRITE);
    if(!ks)
    {
        CRITICAL("KinectOutNode fail create KinectStorage "+value)
    }
}

void KinectOutNode::stop()
{
    //qDebug()<<"Stoped";
    if(0!=ks)
    {
        delete ks;
        ks = 0;
    }
    OutputNode::stop();
}
