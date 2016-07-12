#include "ipcamera.h"
#include <utils/logs.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <QtConcurrent/QtConcurrent>
#include <QTcpSocket>
#include <QUrl>

using namespace io::capture;
using namespace io;

IpCamera::IpCamera(QObject *parent) :
    io::capture::OpenCVCameraNode(parent)
{
    _camera = 0;
    //TODO: Initiation source
}

IpCamera::~IpCamera()
{
    _watcher.waitForFinished();
    if(_camera){
        _camera->release();
        delete _camera;
        _camera =0;
    }
}

void IpCamera::setDeviceid (int /*value*/)
{
    //Do nothing
    return;
}

QString IpCamera::ipadress()
{
    return _url;
}

void IpCamera::setIpadress(QString address)
{
    setUrl(address);
}

bool IpCamera::getFrame(cv::Mat &mat)
{
   /* if(!_watcher.isFinished())
        return false;

    if(!_cap)
        return false;*/

   /* if(_cap->isOpened())
    {
        _cap->read(mat);
        cv::cvtColor(mat,mat,::CV_RGB2BGR);
        return true;
    }*/

    if(_camera){
        qDebug() << "Grabbing";
        _camera->read(mat);
        cv::cvtColor(mat,mat,cv::COLOR_RGB2BGR);
        return true;
    }
    return false;
}

void IpCamera::concurrentOpen(cv::VideoCapture* pCap,QString name)
{
     std::string copy = name.toStdString();
     QTcpSocket socket;
     QUrl url(name);
    // INFO(QString("Connecting to %1").arg(url.toString(QUrl::RemoveScheme|QUrl::StripTrailingSlash|QUrl::RemovePath|QUrl::RemoveQuery).remove("//")))
     socket.connectToHost(url.toString(QUrl::RemoveScheme|QUrl::StripTrailingSlash|QUrl::RemovePath|QUrl::RemoveQuery).remove("//"),80);
     if (socket.waitForConnected(5000))
     {
         socket.close();
         pCap->open(copy);

     }else{
         WARNING("Cannot connect to host")
                 return;
     }
    // INFO("Connected")
}
void IpCamera::setUrl(QString value)
{
     if(_watcher.isRunning())
     {
        _watcher.waitForFinished();
     }
     _url = value;
     if(0!=_camera){
         _camera->release();
         delete _camera;
         _camera = 0;
     }
     if(0==_cap)
     {
         _cap = new cv::VideoCapture();
     }

     emit status(tr("Connecting..."),0,Node::Percent);
	 _watcher.disconnect();
     connect(&_watcher,SIGNAL(finished()),this,SLOT(handleConnection()));
    // QFuture<void> fopen = QtConcurrent::run(IpCamera::concurrentOpen,_cap, value);
    // _watcher.setFuture(fopen);
   //  _camera = new cv::VideoCapture(value.toUtf8());
     handleConnection();
}

void IpCamera::handleConnection()
{
    if(0!=_camera)
    {
        //Считываеми параметры записи
       setLength(0);//cvGetCaptureProperty(_camera,CV_CAP_PROP_FRAME_COUNT));
       setFps(30);//cvGetCaptureProperty(_camera,CV_CAP_PROP_FPS));
       emit status(tr("Connected"),0,Node::Ready);
    }else{

        INFO("Capture not opened "+_url)
        emit status(tr("Error..."),0,Node::Error);
    }
}
