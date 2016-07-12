#include "pointgraycamera.h"


#include <QDebug>
#include <types/rgb.h>
#include <Windows.h>



using namespace FlyCapture2;
using namespace types;

int PointGrayCamera::_ncams;
QList<int> PointGrayCamera::_usedCams;
cv::Mat* PointGrayCamera::_mats[MAX_CAMS];
//FlyCapture2::Image* PointGrayCamera::_ptr[MAX_CAMS];
//QMutex PointGrayCamera::_mutex[MAX_CAMS];

void PointGrayCamera::getFrame()
{
    receive(esp::init<int>(new int));
}

//QList<FlyCapture2::Image *> PointGrayCamera::_currentData;


PointGrayCamera::PointGrayCamera(QObject *parent) :
    io::capture::CameraNode(parent)
{

    _id = -1;
    _ncams = -1;


    _cam = 0;
    _serial = "";
    _vm = FlyCapture2::VIDEOMODE_1600x1200Y8;
    _ncams = numCam();
    setId(_id);
    //_secretdata = 0;

    qDebug() << _usedCams;
}

int PointGrayCamera::numCam() const
{
    if(-1==_ncams)
    {
        BusManager busMgr;
        unsigned int numCameras;
        FlyCapture2::Error error;

        error = busMgr.GetNumOfCameras(&numCameras);


        if (error != PGRERROR_OK)
        {
            return 0;
        }
        return numCameras;
    }
    return _ncams;
}

QString PointGrayCamera::serial()const
{
    return _serial;
}

int PointGrayCamera::id() const
{
    return _id;
}

void PointGrayCamera::setId(const int id)
{
    if(_id==id)
    {
        return;
    }

    if(-1==id)
    {
        if(_cam)
        {
            _usedCams.removeOne(_id);
            _id = -1;
            _cam->StopCapture();
            _cam->Disconnect();
            delete _cam;
            _cam =0;
            if(_mats[_id]){
                delete _mats[_id];
                _mats[_id] = 0;
            }
        }
        return;
    }


    if(_usedCams.contains(id))
    {
        qDebug() << "camera used" <<id << _usedCams;
        return;
    }
    FlyCapture2::Error error;
    PGRGuid guid;

    if(numCam()>id&&id<0)
    {
        qDebug() << "No camera";
        return;
    }

    if(_cam)
    {
      //  qDebug() << "remove cam" << _id;
        _usedCams.removeOne(_id);
        _cam->StopCapture();
        _cam->Disconnect();
        delete _cam;
        _cam =0;
        if(_mats[_id]){
            delete _mats[_id];
            _mats[_id] = 0;
        }
    }

    BusManager busMgr;
    timerx.restart();
    error = busMgr.GetCameraFromIndex(id, &guid);
    CHECK_ERROR(error)

    _cam = new Camera();


    error = _cam->Connect(&guid);

    CHECK_ERROR(error)



    VideoMode mode;
    FrameRate rate;
    error = _cam->GetVideoModeAndFrameRate(&mode,&rate);

    CHECK_ERROR(error)
    videoMode2WH(mode);
    _vm = mode;
    _fr = rate;

    FlyCapture2::CameraInfo info;
    error = _cam->GetCameraInfo(&info);
    FlyCapture2::CameraStats stats;

   // _cam->StartSyncCapture()

    FlyCapture2::Property gain = buildProperty(FlyCapture2::GAIN,6);
    _cam->SetProperty(&gain);
    FlyCapture2::Property brightness = buildProperty(FlyCapture2::BRIGHTNESS ,31);
    _cam->SetProperty(&brightness);
    FlyCapture2::Property fr = buildProperty(FlyCapture2::FRAME_RATE  ,33.333f);
    _cam->SetProperty(&fr);
    FlyCapture2::Property shutter = buildProperty(FlyCapture2::SHUTTER  ,25);
    _cam->SetProperty(&shutter);
    FlyCapture2::Property exposure = buildProperty(FlyCapture2::AUTO_EXPOSURE   ,1.3f);
    _cam->SetProperty(&exposure);
    FlyCapture2::Property sharpness = buildProperty(FlyCapture2::SHARPNESS  ,1015);
    _cam->SetProperty(&sharpness);



    // turn on timestamping
    unsigned int ulRegister = 0x12f8;
    unsigned int ulValue;



    error = _cam->ReadRegister(ulRegister,&ulValue);
    CHECK_ERROR(error)

    //
    // ensure time stamping is present...
    //
    if (!( ulValue & 0x80000000))
    {
          qDebug() << "Camera 1 does not support the timestamp feature - upgrade firmware";
          return;
    }



    _lasttimestamp = -1;
    CHECK_ERROR(error)

    error = _cam->WriteRegister(ulRegister,ulValue | 1);

    CHECK_ERROR(error)

    _serial = QString("%1").arg(info.serialNumber);
    _id = id;
    _usedCams.push_back(_id);



    _mats[_id] = new cv::Mat(height(),width(),CV_8UC1);
    _image = cv::Mat(height(),width(),CV_8UC1);


  //  qDebug() << "camera used" << _usedCams;
    error = _cam->StartCapture(&PointGrayCamera::callBack,&_id);
  //  qDebug() << "camera used" << _usedCams;
    CHECK_ERROR(error)



    /*ulRegister =0x12E0;
    error = _cam->ReadRegister(ulRegister,&ulValue);
    CHECK_ERROR(error)
            qDebug() << ulValue;*/
}

void PointGrayCamera::callBack(Image *pImage, const void *pCallbackData)
{
 //   qDebug() << "camera used" << _usedCams;
    int id = *(int*)(pCallbackData);

  //  qDebug() << id << "recived";// << pImage->GetCols() << pImage->GetRows();
 //   qDebug() << _mats[id]->cols << _mats[id]->rows  ;
    //if(_mutexArray[id].tryLock())
  /*  if(!_mutexArray[id].tryLock())
    {
        return;
    }*/
  //  _mutex[id].lock();
  //  _ptr[id] = pImage;
   // QMutexLocker lock(&_mutex[id]);
 //   _currentData[id] = pImage;
     memcpy(_mats[id]->data,pImage->GetData(),pImage->GetCols()*pImage->GetRows());
 //    _mutex[id].unlock();
 //   _mutexArray[id].unlock();

}

FlyCapture2::Property PointGrayCamera::buildProperty(PropertyType pt,float value,bool autoValue)
{
    FlyCapture2::Property p;
    p.type = pt;
    p.absControl = true;
    p.onePush = false;
    p.onOff = true;
    p.autoManualMode = autoValue;
    p.absValue = value;
    return p;
}

double PointGrayCamera::fps()
{
    if(!_cam)
        return 0;
    FlyCapture2::Property real = buildProperty(FlyCapture2::FRAME_RATE  ,33.333f);
    _cam->GetProperty(&real);
    return real.absValue;
}
void PointGrayCamera::setFps(double value)
{
    if(!_cam)
            return;
    FlyCapture2::Property real = buildProperty(FlyCapture2::FRAME_RATE  ,value);
    _cam->SetProperty(&real);

    FlyCapture2::Error error;
    FC2Config config;
    error = _cam->GetConfiguration( &config );
    CHECK_ERROR(error);
    // Set the grab timeout to 100 mseconds
    config.grabTimeout = 1000.0f/value;
    error = _cam->SetConfiguration( &config );
    CHECK_ERROR(error);

    CameraNode::setFps(value);
}

double PointGrayCamera::shutter(){
    if(!_cam)
        return 0;
    FlyCapture2::Property shutter = buildProperty(FlyCapture2::SHUTTER  ,25);
    _cam->GetProperty(&shutter);
    return shutter.absValue;
}
void PointGrayCamera::setShutter(double value)
{
    if(!_cam)
            return;
    FlyCapture2::Property shutter = buildProperty(FlyCapture2::SHUTTER  ,value);
    _cam->SetProperty(&shutter);
}

PointGrayCamera::VideoModeX PointGrayCamera::videoMode() const
{
    return (PointGrayCamera::VideoModeX)(int)(_vm);
}

void PointGrayCamera::setVideoMode(const VideoModeX vm )
{
    if(_cam)
    {
    /*   FlyCapture2::Error error;
        FlyCapture2::VideoMode test = ( FlyCapture2::VideoMode)(int)(vm);
        if(VIDEOMODE_FORMAT7!=vm)
        {
            if(VIDEOMODE_FORMAT7==_fr)
            {
                _fr = FlyCapture2::FRAMERATE_60;
            }
        }

        error = _cam->SetVideoModeAndFrameRate(test,_fr);
        FlyCapture2::Property fr = buildProperty(FlyCapture2::FRAME_RATE  ,120.f,true);
        _cam->SetProperty(&fr);

        CHECK_ERROR(error)
        _vm = test;
        videoMode2WH(test);*/

    }
}

void PointGrayCamera::videoMode2WH(FlyCapture2::VideoMode vm)
{
    switch(vm)
    {
    case VIDEOMODE_160x120YUV444: setWidth(260); setHeight(120); break;

    case VIDEOMODE_320x240YUV422: setWidth(320); setHeight(240); break;

    case VIDEOMODE_640x480YUV411: setWidth(640); setHeight(480); break;

    case VIDEOMODE_640x480YUV422: setWidth(640); setHeight(480); break;

    case VIDEOMODE_640x480RGB: setWidth(640); setHeight(480);break;

    case VIDEOMODE_640x480Y8: setWidth(640); setHeight(480); break;

    case VIDEOMODE_640x480Y16: setWidth(640); setHeight(480); break;

    case VIDEOMODE_800x600YUV422  :setWidth(800); setHeight(600); break;

    case VIDEOMODE_800x600RGB  :setWidth(800); setHeight(600); break;

    case VIDEOMODE_800x600Y8  :setWidth(800); setHeight(600); break;

    case VIDEOMODE_800x600Y16  :setWidth(800); setHeight(600); break;

    case VIDEOMODE_1024x768YUV422  :setWidth(1024); setHeight(768); break;

    case VIDEOMODE_1024x768RGB  :setWidth(1024); setHeight(768); break;

    case VIDEOMODE_1024x768Y8  :setWidth(1024); setHeight(768); break;

    case VIDEOMODE_1024x768Y16  :setWidth(1024); setHeight(768); break;

    case VIDEOMODE_1280x960YUV422  :setWidth(1280); setHeight(960); break;

    case VIDEOMODE_1280x960RGB  :setWidth(1280); setHeight(960); break;

    case VIDEOMODE_1280x960Y8  :setWidth(1280); setHeight(960); break;

    case VIDEOMODE_1280x960Y16 :setWidth(1280); setHeight(960); break;

    case VIDEOMODE_1600x1200YUV422  :setWidth(1600); setHeight(1200); break;

    case VIDEOMODE_1600x1200RGB  :setWidth(1600); setHeight(1200); break;

    case VIDEOMODE_1600x1200Y8  :setWidth(1600); setHeight(1200); break;

    case VIDEOMODE_1600x1200Y16  :setWidth(1600); setHeight(1200); break;

    case VIDEOMODE_FORMAT7  : if(_cam){
            FlyCapture2::Format7Info f7i;
            bool supported;
            _cam->GetFormat7Info(&f7i,&supported);
            setWidth( f7i.maxWidth); setHeight( f7i.maxHeight);
            } break;


    }
}

PointGrayCamera::FrameRateX PointGrayCamera::frameRate() const
{
    return (PointGrayCamera::FrameRateX)(int)_fr;
}

void PointGrayCamera::setFrameRate(const FrameRateX fr)
{
    if(_cam)
    {
        FlyCapture2::Error error;
        FlyCapture2::FrameRate test = (FlyCapture2::FrameRate)(int)fr;
        error = _cam->SetVideoModeAndFrameRate( _vm, test);
        CHECK_ERROR(error)
        _fr = test;
        frameRate2Fps(test);

    }
}

PointGrayCamera::~PointGrayCamera()
{
    if(_cam)
    {
       /* if(_secretdata)
        {
           _image.data = _secretdata;
        }*/
        _usedCams.removeOne(_id);
        _cam->StopCapture();
        _cam->Disconnect();
        delete _cam;
        if(_mats[_id]){
            delete _mats[_id];
            _mats[_id] = 0;
        }
     //   _mutex[_id].unlock();
       // if()
      //  delete _mats[_id];
      //  _mats[_id]=0;
    }
}

void PointGrayCamera::PrintError( FlyCapture2::Error error )
{
    qDebug() << "Error" ;
    error.PrintErrorTrace();
}
void PointGrayCamera::frameRate2Fps(FlyCapture2::FrameRate fr)
{
    switch(fr)
    {
    case FRAMERATE_1_875: setFps(1.875); break; /**< 1.875 fps. */
    case FRAMERATE_3_75:setFps(3.75); break; /**< 3.75 fps. */
    case FRAMERATE_7_5: setFps(7.5);break; /**< 7.5 fps. */
    case FRAMERATE_15: setFps(15); break; /**< 15 fps. */
    case FRAMERATE_30:setFps(30); break; /**< 30 fps. */
    case FRAMERATE_60:setFps(60); break; /**< 60 fps. */
    case FRAMERATE_120:setFps(120); break; /**< 120 fps. */
    case FRAMERATE_240:setFps(240); break; /**< 240 fps. */
    case FRAMERATE_FORMAT7: if(_cam){

            } break; /**< Custom frame rate for Format7 functionality. */
    case NUM_FRAMERATES :break;
    }
}

inline double imageTimeStampToSeconds(unsigned int uiRawTimestamp)
{

   int nSecond      = (uiRawTimestamp >> 25) & 0x7F;   // get rid of cycle_* - keep 7 bits
   int nCycleCount  = (uiRawTimestamp >> 12) & 0x1FFF; // get rid of offset
   int nCycleOffset = (uiRawTimestamp >>  0) & 0xFFF;  // get rid of *_count

   return (double)nSecond + (((double)nCycleCount+((double)nCycleOffset/3072.0))/8000.0);
}

inline double readTimeStamp(unsigned char* data)
{
   /* strictly little endian... */
   unsigned int stamp;
   unsigned char* pStamp = (unsigned char*)&stamp;
   pStamp[0] = data[3];
   pStamp[1] = data[2];
   pStamp[2] = data[1];
   pStamp[3] = data[0];

   int nSecond      = (stamp >> 25) & 0x7F;   // get rid of cycle_* - keep 7 bits
   int nCycleCount  = (stamp >> 12) & 0x1FFF; // get rid of offset
   int nCycleOffset = (stamp >>  0) & 0xFFF;  // get rid of *_count

   return (double)nSecond + (((double)nCycleCount+((double)nCycleOffset/3072.0))/8000.0);
}

esp PointGrayCamera::get(const base::timestamp framex)
{
   // esp result();
    if(_cam)
    {
        base::timestamp frame = framex;
      //  qDebug() << objectName() << "My id" << GetCurrentThreadId();
      //  Image rawImage;
        FlyCapture2::TimeStamp ts;

      //  _mutex[_id].lock();
       // memcpy(_mats[_id]->data,_ptr[_id]->GetData(),_ptr[_id]->GetCols()*_ptr[_id]->GetRows());
       // _mutex[_id].unlock();
    //    qDebug() << _currentData[_id]->GetRows()  << _currentData[_id]->GetCols();
    /*    double t1 = cv::getTickCount();
        FlyCapture2::Error error = _cam->RetrieveBuffer( &rawImage );
        qDebug() << (cv::getTickCount() - t1)/cv::getTickFrequency();
        if (error == PGRERROR_OK)
        {
            memcpy(_image.data,rawImage.GetData(),rawImage.GetCols()*rawImage.GetRows());
            SEND(Rgb, new Rgb(_image))
        }*/

     /*   if(0!=_currentData[_id]){
          //  QMutexLocker lock(&_mutexArray[_id]);
            qDebug() << _id;// << _currentData[_id]->GetRows() << _currentData[_id]->GetCols();
            if(_image.rows!=_currentData[_id]->GetRows()||_image.cols!=_currentData[_id]->GetCols())
            {
                if(_secretdata)
                {
                   _image.data = _secretdata;
                }
                _image = cv::Mat(_currentData[_id]->GetRows(),_currentData[_id]->GetCols(),CV_8UC1);
                _imsize2 = _currentData[_id]->GetRows()*_currentData[_id]->GetCols();
            }*/

         //   memcpy(_image.data,_currentData[_id]->GetData(),_imsize2);

       // }

   /*     if (error != PGRERROR_OK)
        {
            qDebug() << frame << "dropped camera"<< objectName();;
         //   SEND(Rgb, new Rgb(cv::Mat()))
            return  esp();
        }*/

     /*   error = _cam->GetCycleTime(&ts);
        if (error != PGRERROR_OK)
        {
            error.PrintErrorTrace();
        }*/

    //    qDebug() << (double)ts.seconds + (((double)ts.cycleCount+((double)ts.cycleOffset/3072.0))/8000.0);



    /*     double timestamp = readTimeStamp(rawImage.GetData());
       //  qDebug() << _lasttimestamp << timestamp << 1.0/fps() << timestamp-_lasttimestamp;
         if(-1!=_lasttimestamp)
         {
             if(timestamp-_lasttimestamp>(2.0/fps()))
             {
               //  frame = frame-1;
             //    qDebug() << frame << "dropped timestamp" << objectName() << timestamp-_lasttimestamp;
              //   qDebug() << "Error times" << floor((timestamp-_lasttimestamp)/(1.0/fps()));
              //    _lasttimestamp = timestamp;
            //     SEND(Rgb, new Rgb(cv::Mat()))
              //   return  esp();
             }
         }
         _lasttimestamp = timestamp;*/

        /* if (error != PGRERROR_OK)
                 {
                     PrintError( error );
                      return  esp();
                 }*/


     //   double timer_s =  (double)timerx.nsecsElapsed()/1000000000.0f;
       // qDebug() << objectName().toInt() <<'\t'<< readTimeStamp(rawImage.GetData()) <<'\t'<< timer_s-readTimeStamp(rawImage.GetData());
                // Create a converted image


      /*                  // Convert the raw image
         error = rawImage.Convert( PIXEL_FORMAT_MONO8, &convertedImage );
                        if (error != PGRERROR_OK)
                        {
                            PrintError( error );
                            return  esp();
                        }*/

       //  cv::Mat bitmap(convertedImage.GetRows(),convertedImage.GetCols(),CV_8UC1);
       //  unsigned int imsize = (convertedImage.GetRows()+convertedImage.GetStride())*convertedImage.GetCols();
       //  unsigned int imsize2 = convertedImage.GetRows()*convertedImage.GetCols();
       //  memcpy(bitmap.data,convertedImage.GetData(),imsize2);
        //_image.data = convertedImage.GetData();
         // = esp::init<Rgb>(new Rgb(bitmap),frame);
        if(_mats[_id])
        {
            SEND(Rgb, new Rgb(*_mats[_id]))
        }


                     //   convertedImage.Save( "D:/devel.bmp" );
    }
    return esp();
}
