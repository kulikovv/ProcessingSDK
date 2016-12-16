#include "kinectopenni.h"
#include <QDebug>
#include <QMutex>
#include <QWriteLocker>
#include <types/rgbd.h>
#include <opencv2/core/core.hpp>
#include <utils/logs.h>
#include "rgbdkinect.h"

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

KinectOpenNI::KinectOpenNI(QObject *parent)
        : capture::SingleCameraNode(parent)
{


	m_high_resolution=false;
    m_mirrored=false;
	_connected=false;
	_serial = "";
    _deviceid = -1;
    _proj = 0;
    _real  = 0;
    _sendKinectFrame = true;
    _sendPointCloud = false;
    _send3d = false;
    setDeviceid(_deviceid);


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



    if(_proj){
        delete _proj;
        _proj = 0;

    }
    if(_real){
        delete _real;
        _real = 0;
    }

}
void KinectOpenNI::disconnect()
{
	QMutexLocker locker(&_mutex);
	if(!_connected)
		return;
	 
	g_depth.StopGenerating();
    g_image.StopGenerating();
    
    g_depth.Release();
    g_image.Release();
    g_device.Release();
    _connected = false;
	_serial = "";

    g_context.StopGeneratingAll();
    g_context.Release();

    int index = _kinectlist.indexOf(_deviceid);
    if(-1!=index)
    {
        _kinectlist.removeAt(index);
    }
}
esp KinectOpenNI::get(const base::timestamp frame)
{
   if(!_connected)
   {
        return esp();
   }
 //  QMutexLocker locker(&_mutex);

   g_depth.GetMetaData(g_depthMD);
   g_image.GetMetaData(g_imageMD);

   cv::Mat rgb;
   cv::Mat depth;


   const XnDepthPixel* pDepth = g_depth.GetDepthMap();//g_depthMD.Data();
   depth.create(480,640,CV_16UC1);
   assert((g_depthMD.XRes() == depth.cols) && (g_depthMD.YRes() == depth.rows)); 
   memcpy(depth.data,pDepth,640*480*2);

   const XnUInt8* pImage = g_imageMD.Data();
   rgb.create(480,640,CV_8UC3);
   assert(g_imageMD.PixelFormat() == XN_PIXEL_FORMAT_RGB24);
   memcpy(rgb.data,pImage,640*480*3);

   XnUInt64 F;
   XnDouble pixel_size;
   // get the focal length in mm (ZPS = zero plane distance)
   g_depth.GetIntProperty ("ZPD", F);
   // get the pixel size in mm ("ZPPS" = pixel size at zero plane)
   g_depth.GetRealProperty ("ZPPS", pixel_size);
   //qDebug() << F << pixel_size;

   RgbdKinect *kf = new RgbdKinect(pixel_size,F,_serial,rgb,depth,cv::Point3d(0,0,0));
   SEND(types::Rgbd,kf)

 /*  if(_sendKinectFrame)
   {
       const XnUInt8* pImage = g_imageMD.Data();
       rgb.create(480,640,CV_8UC3);
       assert(g_imageMD.PixelFormat() == XN_PIXEL_FORMAT_RGB24);
       memcpy(rgb.data,pImage,640*480*3);

       if(_send3d)
       {
           depth = retrievePointCloudAsImage(pDepth);
       }else{
           depth.create(480,640,CV_16UC1);
           memcpy(depth.data,pDepth,640*480*2);
       }
       types::Rgbd *kf = new types::Rgbd(_serial,rgb,depth,cv::Point3d(0,0,0));
       if(_send3d)
       {
           SEND_TAG(types::Rgbd,kf,"3D")
       }else{
           SEND(types::Rgbd,kf)
       }
   }
   if(_sendPointCloud)
   {
//       //qDebug() << "Send pc";
       SEND(PointCloud,xnPoint3DToPointCloud(retrievePointCloudMap(pDepth),640*480))
   }*/
   //xnWaitNoneUpdateAll
 //  g_context.WaitNoneUpdateAll();
   g_context.WaitAndUpdateAll();
   return esp();

}

void KinectOpenNI::receiveProcess(esp data)
{
	get(data.getFrameId());
}
/*
bool KinectOpenNI::sendKinectFrame()
{
    return _sendKinectFrame;
}

bool KinectOpenNI::sendPointCloud()
{
    return _sendPointCloud;
}

void KinectOpenNI::setSendKinectFrame(bool value)
{
    _sendKinectFrame = value;
}

void KinectOpenNI::setSendPointCloud(bool value)
{
    _sendPointCloud = value;
}*/

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

    XnStatus status = g_context.Init();

    checkXnError(status, "Initialize context");



    //XnStatus status;
    xn::NodeInfoList device_node_info_list;
    status = g_context.EnumerateProductionTrees(XN_NODE_TYPE_DEVICE, NULL, device_node_info_list);
    checkXnError(status, "Cannot enumerate devices.");
    xn::NodeInfoList::Iterator nodeIt = device_node_info_list.Begin();
    int current_id = 0;
    for (; current_id < id && nodeIt != device_node_info_list.End (); ++nodeIt, ++current_id) {}
    
    xn::NodeInfo deviceInfo = *nodeIt;


	
    status = g_context.CreateProductionTree(deviceInfo, g_device);
    checkXnError(status, "Create Device Node");

    const XnProductionNodeDescription& description = deviceInfo.GetDescription();

	


    #ifdef UNIX_BUILD
        _serial = getKinectSerial(id);
    #else
    char* infoChr = (char*)deviceInfo.GetCreationInfo();
    if(0!=infoChr)
    {
        QString info(infoChr);
        QStringList list = info.split('#');
        if(list.count()>2)
        {
            _serial = list[2].toUpper();
            //eebug() << description.strVendor << description.strName << deviceInfo.GetInstanceName() << _serial ;
        }else{
            _serial = info;
        }

    }else{
        _serial = "Unknown";
    }
    #endif
   // //qDebug() << "Brk" << infoChr;


    xn::Query query;
    query.AddNeededNode(deviceInfo.GetInstanceName());

    status = g_depth.Create(g_context, &query);
    checkXnError(status, "Create depth generator");
    XnMapOutputMode depth_mode;
    depth_mode.nXRes = 640;
    depth_mode.nYRes = 480;
    _fps = depth_mode.nFPS = 30;
    g_depth.SetMapOutputMode(depth_mode);

    status = g_image.Create(g_context, &query);
    checkXnError(status, "Create image generator");

    status = g_image.SetIntProperty ("Resolution", 1);
    checkXnError(status, "Resolution");

    status = g_context.SetGlobalMirror(m_mirrored);
    checkXnError(status, "Mirror");

    XnMapOutputMode rgb_mode;
    if (m_high_resolution)
    {
        _fps = rgb_mode.nFPS = 15;
        rgb_mode.nXRes = 1280;
        rgb_mode.nYRes = 1024;
    }
    else
    {
        rgb_mode.nXRes = 640;
        rgb_mode.nYRes = 480;
        _fps = rgb_mode.nFPS = 30;
    }
    g_image.SetMapOutputMode(rgb_mode);

    status = g_depth.GetAlternativeViewPointCap().SetViewPoint(g_image);
  //  status = g_image.GetAlternativeViewPointCap().SetViewPoint(g_depth);
    checkXnError(status, "Depth::GetAlternativeViewPointCap");

    status = g_depth.StartGenerating();
    checkXnError(status, "Depth::StartGenerating");

    status = g_image.StartGenerating();
    checkXnError(status, "RGB::StartGenerating");

    //Preallocation of memory for converting
    if(_proj){
        delete _proj;
        _proj = 0;

    }
    if(_real){
        delete _real;
        _real = 0;
    }
    _proj = new XnPoint3D[depth_mode.nXRes*depth_mode.nYRes];
    for( int y = 0; y < depth_mode.nYRes; y++ )
    {
        for( int x = 0; x < depth_mode.nXRes; x++ )
        {
            int ind = y*depth_mode.nXRes+x;
            _proj[ind].X = (float)x;
            _proj[ind].Y = (float)y;
            _proj[ind].Z = 0;
            //_proj[ind].Z = depth.at<unsigned short>( y, x);
        }
    }

    _real = new XnPoint3D[depth_mode.nXRes*depth_mode.nYRes];

	g_context.WaitAndUpdateAll();

	_deviceid = id;
	_connected = true;
    _kinectlist.push_back(_deviceid);
	}catch(std::exception &ex){
		CRITICAL(ex.what());
	}catch(...){
		CRITICAL("Unknown exception");
	}

}

void KinectOpenNI::checkXnError(const XnStatus& status, const char* what) const
{
    if (status != XN_STATUS_OK)
    {

		qDebug() << "Error" << what << xnGetStatusString(status);
		throw 1;
    }
}

QString KinectOpenNI::serial(){
	return _serial;
}
void KinectOpenNI::setSerial(QString value)
{
}
cv::Mat KinectOpenNI::retrievePointCloudAsImage(const XnDepthPixel *depth)
{
      cv::Mat out(480,640,CV_32FC3,cv::Scalar::all(0));
   /*   cv::Mat img(480,640,CV_32FC1,cv::Scalar::all(0));
      cv::Mat imgx(480,640,CV_32FC1,cv::Scalar::all(0));
      cv::Mat imgy(480,640,CV_32FC1,cv::Scalar::all(0));*/

      XnPoint3D *cloud = retrievePointCloudMap(depth);
      memcpy(out.data,cloud,sizeof(float)*480*640);
    //  out*=0.001f;
    //  out = out*0.001f;
    /*  for(int i=0;i<480*640;i++)
      {
            img.at<float>(i/640,i%640) = cloud[i].Z*0.001f;
            imgx.at<float>(i/640,i%640) = cloud[i].X*0.001f;
            imgy.at<float>(i/640,i%640) = cloud[i].Y*0.001f;
      }*/
/*
      std::vector<cv::Mat> planes;
      planes.push_back(img);
      planes.push_back(imgx);
      planes.push_back(imgy);
      cv::merge(planes,out);*/
      return out;
}

XnPoint3D *KinectOpenNI::retrievePointCloudMap(const XnDepthPixel* depth)
{
    int cols = 640;
    int rows = 480;

    if( cols <= 0 || rows <= 0 ){
        return 0;
    }

    if(0==_proj||0==_real)
    {
        qDebug() << "Nothing!";
        return 0;
    }


 /*   std::vector<cv::Point3f> *data = new std::vector<cv::Point3f>(cols*rows);
    cv::Ptr<XnPoint3D> proj = new XnPoint3D[cols*rows];
    cv::Ptr<XnPoint3D> real = new XnPoint3D[cols*rows];*/
  /*  for( int y = 0; y < rows; y++ )
    {
        for( int x = 0; x < cols; x++ )
        {
            int ind = y*cols+x;
            _proj[ind].X = (float)x;
            _proj[ind].Y = (float)y;
            _proj[ind].Z = depth.at<unsigned short>( y, x);
        }
    }*/
  //  unsigned short* dptr = depth.ptr<unsigned short>(0);
    XnUInt64 F_;
    XnDouble pixel_size_;

  //  double t1 = cv::getTickCount();
    const XnDepthPixel* depth_md_ = g_depth.GetDepthMap();
 //   double t2 = cv::getTickCount();

    // get the focal length in mm (ZPS = zero plane distance)
    g_depth.GetIntProperty ("ZPD", F_);
    // get the pixel size in mm ("ZPPS" = pixel size at zero plane)
    g_depth.GetRealProperty ("ZPPS", pixel_size_);

    double tempm = pixel_size_ * 0.001 / F_;
    for (int i=0;i<rows*cols;i++)
    {
        _real[i].X = (_proj[i].X - 320) * depth_md_[i] * tempm;
        _real[i].Y = (_proj[i].Y - 240) * depth_md_[i] * tempm;
        _real[i].Z = depth_md_[i] * 0.001f; // from mm in meters!
    }

  //  qDebug() << "Hardway" << (cv::getTickCount()-t1)/cv::getTickFrequency();
 //   qDebug() << "Hardway only pcoress" << (cv::getTickCount()-t2)/cv::getTickFrequency();

 /*   t1 = cv::getTickCount();
    for (int i=0;i<rows*cols;i++)
    {
        _proj[i].Z = depth[i];
    }
    g_depth.ConvertProjectiveToRealWorld(cols*rows, _proj, _real);
    qDebug() << "Easyway" << (cv::getTickCount()-t1)/cv::getTickFrequency();*/

   /* for(int i=0;i<cols*rows;i++)
    {
        if( 0==depth.at<unsigned short>(i/cols, i%cols) ) // not valid
        {
            _cloud->at(i)  = cv::Point3f( 0, 0, 0 );
        }else{
            data->at(i) = cv::Point3f( real[i].X*0.001f, real[i].Y*0.001f, real[i].Z*0.001f); // from mm to meters
        }

    }*/
    return _real;

}
cv::vector<cv::Point3f>* KinectOpenNI::xnPoint3DToPointCloud(XnPoint3D *real,int size)
{
    if(!real)
    {
        return 0;
    }

    std::vector<cv::Point3f> *data = new std::vector<cv::Point3f>(size);
    for(int i=0;i<size;i++)
        {
        /*    if( 0==depth.at<unsigned short>(i/cols, i%cols) ) // not valid
            {
                data->at(i)  = cv::Point3f( 0, 0, 0 );
            }else{*/
                data->at(i) = cv::Point3f( real[i].X*0.001f, real[i].Y*0.001f, real[i].Z*0.001f); // from mm to meters
          //  }

        }
    return data;
}
