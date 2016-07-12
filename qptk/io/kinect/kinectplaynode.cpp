#include "kinectplaynode.h"
#include <io/kinect/kinectstorage.h>
#include <types/rgbd.h>
#include <utils/logs.h>
#include <QFile>
#include <QFileInfo>
using namespace io;
using namespace io::kinect;
using namespace types;
using namespace base;

KinectPlayNode::KinectPlayNode(QObject *parent) :
    SourceNode(parent)
{
    _extension = "kin";
    ks=0;
    postedframes=0;
    _fps = 25;
    _length = 0;
    _readserial = true;
}
KinectPlayNode::~KinectPlayNode()
{
    if(ks)
    {
        delete ks;
        ks=0;
    }
}

esp KinectPlayNode::get(const unsigned long frame)
{
    if(!ks){
        return esp();
    }

    types::Rgbd* kf=0;
    postedframes = frame;
    if(0!=ks->length())
    {
        postedframes= frame%ks->length();
    }
    kf = ks->read(postedframes);
    if(!kf)
    {


        return esp();
    }


    SEND(types::Rgbd,kf)
    return esp();
}

void KinectPlayNode::receiveProcess(esp data)
{
    get(data.getFrameId());
}

void KinectPlayNode::setUrl(QString value)
{

    QFileInfo fi(value);

    if(!fi.exists()||fi.isDir()||0!=QString::compare(fi.completeSuffix(),_extension))
    {
        _url = value;
        return;
    }
    if(ks)
    {
        delete ks;
        ks=0;
    }
    try{
        ks = new KinectStorage(value.toStdString(),KinectStorage::READ);
        if (0!=ks)
        {
            _url = value;
            _length = ks->length();
            _fps = ks->getFps();
        }
    }catch(cv::Exception ex){
        CRITICAL("Exception: " + QString::fromStdString(ex.msg))
    }
}
bool KinectPlayNode::readSerial()
{
    return _readserial;
}

void KinectPlayNode::setReadSerial(bool rs)
{
    _readserial = rs;
}

