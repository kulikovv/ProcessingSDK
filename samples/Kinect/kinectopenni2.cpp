#include "kinectopenni2.h"
#include <QDebug>
#include <QMutex>
#include <QWriteLocker>
#include <types/rgbd.h>
#include <opencv2/core/core.hpp>
#include <utils/logs.h>
#include "rgbdkinect.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef UNIX_BUILD
#include "kinectserial.hpp"
#endif

typedef std::vector<cv::Point3f> PointCloud;
QMutex KinectOpenNI::_mutex;

QList<int> KinectOpenNI::_kinectlist;

QIcon KinectOpenNI::getIcon()
{
    return QIcon(":/kinectlibrary/KinectOpenNI");
}
bool KinectOpenNI::flip(){
    return _flip;
}
void KinectOpenNI::setflip(bool flip)
{
    _flip = flip;
}

KinectOpenNI::KinectOpenNI(QObject *parent)
        : capture::SingleCameraNode(parent)
{

    openni::OpenNI::initialize();
	m_high_resolution=false;
    m_mirrored=false;
	_connected=false;
	_serial = "";
    _deviceid = -1;
    setDeviceid(0);


   // _cloud = 0;

	
}

bool KinectOpenNI::getFrame(cv::Mat &mat)
{
    return false;
}

bool KinectOpenNI::isConnected()
{
    return _connected;
}

/*bool KinectOpenNI::send3D()
{
    return _send3d;
}

void KinectOpenNI::setSend3D(bool value)
{
    _send3d = value;
}*/

KinectOpenNI::~KinectOpenNI()
{
	disconnect();
    openni::OpenNI::shutdown();
}
void KinectOpenNI::disconnect()
{
	QMutexLocker locker(&_mutex);
	if(!_connected)
		return;
	 

    _connected = false;
	_serial = "";

    _depth.stop();
    _color.stop();
    _depth.destroy();
    _color.destroy();


    int index = _kinectlist.indexOf(_deviceid);
    if(-1!=index)
    {
        _kinectlist.removeAt(index);
    }
    _device.close();
}
esp KinectOpenNI::get(const base::timestamp frame)
{
   if(!_connected)
   {
        return esp();
   }
 //  QMutexLocker locker(&_mutex);

   _depth.readFrame(&_depthFrame);
   _color.readFrame(&_colorFrame);

   cv::Mat rgb;
   cv::Mat depth;

   depth.create(480,640,CV_16UC1);
   memcpy(depth.data,_depthFrame.getData(),2*640*480);

   if(_flip){
       cv::flip(depth,depth,1);
   }
//   cv::Mat f;
 //  depth.convertTo(rgb,CV_32FC1);
  // cv::
 //  depth.convertTo(rgb,CV_8UC3);
   rgb.create(480,640,CV_8UC3);
   memcpy(rgb.data,_colorFrame.getData(),640*480*3);

   int F=120;
   float pixel_size=0.1042;


   // get the focal length in mm (ZPS = zero plane distance)
 //  g_depth.GetIntProperty ("ZPD", F);
   // get the pixel size in mm ("ZPPS" = pixel size at zero plane)
 //  g_depth.GetRealProperty ("ZPPS", pixel_size);
   //qDebug() << F << pixel_size;

   RgbdKinect *kf = new RgbdKinect(pixel_size,F,_serial,rgb,depth,cv::Point3d(0,0,0));
   SEND(types::Rgbd,kf)

   return esp();

}

void KinectOpenNI::receiveProcess(esp data)
{
	get(data.getFrameId());
}

void KinectOpenNI::setDeviceid(int id)
{
	try{


    if(id==_deviceid)
    {
        return;
    }

    if(_kinectlist.contains(id))
    {
        return;
    }

    disconnect();



    QMutexLocker locker(&_mutex);

    openni::Status status;

    status = _device.open(openni::ANY_DEVICE);
    checkXnError(status, "Initialize context");





	


    #ifdef UNIX_BUILD
        _serial = getKinectSerial(id);
    #else
        _serial = _device.getDeviceInfo().getUri();
    #endif
   // //qDebug() << "Brk" << infoChr;


    status = _depth.create(_device,openni::SENSOR_DEPTH);
    checkXnError(status, "Create depth generator");

    status = _color.create(_device,openni::SENSOR_COLOR);
    checkXnError(status, "Create color generator");

    _device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);



    status = _depth.start();
    if(openni::STATUS_OK!=status)
    {
        _depth.destroy();
        checkXnError(status, "Depth::StartGenerating");
    }


    status = _color.start();
    if(openni::STATUS_OK!=status)
    {
        _color.destroy();
        checkXnError(status, "RGB::StartGenerating");
    }





	_deviceid = id;
	_connected = true;
    _kinectlist.push_back(_deviceid);
	}catch(std::exception &ex){
		CRITICAL(ex.what());
	}catch(...){
        openni::OpenNI::shutdown();
		CRITICAL("Unknown exception");
	}

}

void KinectOpenNI::checkXnError(openni::Status &status, const char* what) const
{
    if (status != openni::STATUS_OK)
    {

        qDebug() << "Error" << what << openni::OpenNI::getExtendedError();
		throw 1;
    }
}

QString KinectOpenNI::serial(){
	return _serial;
}
void KinectOpenNI::setSerial(QString value)
{
}



