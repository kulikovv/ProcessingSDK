#include "humantrackerimpl.h"
#include <utils/graphdom.h>
#include <types/general_tracking.h>
#include <QFile>

MessageInterceptor::MessageInterceptor(TrackerCallback callback)
{
    _callback = callback;
}

void MessageInterceptor::onGraphData(esp data)
{

    std::map<int,Point3d> coords;
    QSharedPointer<types::TrackedObjects> to = data.getReadOnly<types::TrackedObjects>();
    if(!to.isNull()){
        for (types::TrackedObjects::iterator iter = to->begin(); iter != to->end(); ++iter) {
            coords[iter->first]=Point3d(iter->second.x,iter->second.y,iter->second.z);
        }
    }
    QSharedPointer<types::TrackedObjects3d> to3d= data.getReadOnly<types::TrackedObjects3d>();
    if(!to3d.isNull()){
        for (types::TrackedObjects3d::iterator iter = to3d->begin(); iter != to3d->end(); ++iter) {
            coords[iter->first]=Point3d(iter->second.center.x,iter->second.center.y,iter->second.center.z);
        }
    }
    _callback(coords);
}
HumanTrackerImpl::HumanTrackerImpl(TrackerCallback callback, double thresh):_int(callback)
{
    if(!QFile::exists(":/graphs/track"))
    {
        qDebug() << "File not found";
        throw -1;
    }

    utils::GraphXml::loadNodeLibrary("sdk.dll");
    _sync = utils::GraphDom::loadGraph(":/graphs/track");//:/graphs/track");
    if(!_sync)
    {
        throw -1;
    }

    setThreshold(thresh);
    base::Node* node = _sync->node("kinect");
    if(node){
        node->setProperty("deviceid",0);
    }
    node = _sync->node("Detect");
    if(node){
        QObject::connect(node,SIGNAL(send(esp)),&_int,SLOT(onGraphData(esp)));
    }
}

void HumanTrackerImpl::setThreshold(double thresh)
{
    if(_sync)
    {
        base::Node* node = _sync->node("Plane");
        if(node){
            node->setProperty("minMeters",thresh);
        }


    }
}

HumanTrackerImpl::~HumanTrackerImpl()
{
    if(_sync){
        delete _sync;
        _sync = 0;
    }
}

void HumanTrackerImpl::start()
{
    if(_sync){
        _sync->start();
    }
}

void HumanTrackerImpl::stop()
{
    if(_sync){
        _sync->stop();
    }
}

void HumanTrackerImpl::reset()
{
    if(_sync){
        _sync->reset();
    }
}
