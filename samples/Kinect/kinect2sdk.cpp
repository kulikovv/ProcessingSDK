#include "kinect2sdk.h"
#include "rgbdkinect.h"
// Safe release for interfaces
template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
    if (pInterfaceToRelease != NULL)
    {
        pInterfaceToRelease->Release();
        pInterfaceToRelease = NULL;
    }
}

Kinect2SDK::Kinect2SDK(QObject *parent): capture::SingleCameraNode(parent)
{
    _pKinectSensor=0;
    _pMultiSourceFrameReader=0;
    _connected = false;
}

QIcon Kinect2SDK::getIcon()
{
    return QIcon(":/kinectlibrary/KinectOpenNI");
}

esp Kinect2SDK::get(const base::timestamp frame)
{
   if(!_connected)
   {
        return esp();
   }
   if (!_pMultiSourceFrameReader)
   {
       return esp();
   }

   cv::Mat color,depth;

   IMultiSourceFrame* pMultiSourceFrame = NULL;
   IDepthFrame* pDepthFrame = NULL;
   IColorFrame* pColorFrame = NULL;

   HRESULT hr =0;
   hr = _pMultiSourceFrameReader->AcquireLatestFrame(&pMultiSourceFrame);
   if (FAILED(hr)){
       qDebug() << "Pending";
   }

   if (SUCCEEDED(hr))
   {
       IDepthFrameReference* pDepthFrameReference = NULL;

       hr = pMultiSourceFrame->get_DepthFrameReference(&pDepthFrameReference);
       if (SUCCEEDED(hr))
       {
           hr = pDepthFrameReference->AcquireFrame(&pDepthFrame);
       }

       SafeRelease(pDepthFrameReference);
   }
   if(FAILED(hr)){
       qDebug() << "Depth fail";
   }

   if (SUCCEEDED(hr))
   {
       IColorFrameReference* pColorFrameReference = NULL;

       hr = pMultiSourceFrame->get_ColorFrameReference(&pColorFrameReference);
       if (SUCCEEDED(hr))
       {
           hr = pColorFrameReference->AcquireFrame(&pColorFrame);
       }

       SafeRelease(pColorFrameReference);
   }
   if(FAILED(hr)){
       qDebug() << "Color fail";
   }

   if (SUCCEEDED(hr))
   {

       IFrameDescription* pFrameDescription = NULL;
       IFrameDescription* pColorFrameDescription = NULL;

       INT64 nTime = 0;

       int nWidth = 0;
       int nHeight = 0;
       UINT nBufferSize = 0;


       int nColorWidth = 0;
       int nColorHeight = 0;
       UINT nColorBufferSize = 0;
       ColorImageFormat imageFormat = ColorImageFormat_None;

       USHORT nDepthMinReliableDistance = 0;
       USHORT nDepthMaxDistance = 0;

       UINT16 *pBuffer = NULL;
       RGBQUAD *pColorBuffer = NULL;

       hr = pDepthFrame->get_RelativeTime(&nTime);

       hr = pDepthFrame->get_FrameDescription(&pFrameDescription);

       if (SUCCEEDED(hr))
       {
           hr = pFrameDescription->get_Width(&nWidth);
       }

       if (SUCCEEDED(hr))
       {
           hr = pFrameDescription->get_Height(&nHeight);
       }

       if (SUCCEEDED(hr))
       {
           hr = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
       }

       if (SUCCEEDED(hr))
       {
           // In order to see the full range of depth (including the less reliable far field depth)
           // we are setting nDepthMaxDistance to the extreme potential depth threshold
           nDepthMaxDistance = USHRT_MAX;

           // Note:  If you wish to filter by reliable depth distance, uncomment the following line.
           //// hr = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxDistance);
       }

       if (SUCCEEDED(hr))
       {
           hr = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
       }

       // get color frame data

       if (SUCCEEDED(hr))
       {
           hr = pColorFrame->get_FrameDescription(&pColorFrameDescription);
       }

       if (SUCCEEDED(hr))
       {
           hr = pColorFrameDescription->get_Width(&nColorWidth);
       }

       if (SUCCEEDED(hr))
       {
           hr = pColorFrameDescription->get_Height(&nColorHeight);
       }

       if (SUCCEEDED(hr))
       {
           hr = pColorFrame->get_RawColorImageFormat(&imageFormat);
       }

       if (SUCCEEDED(hr))
       {
           color = cv::Mat(nColorHeight,nColorWidth,CV_8UC4);

           if (imageFormat == ColorImageFormat_Bgra)
           {
               hr = pColorFrame->AccessRawUnderlyingBuffer(&nColorBufferSize, reinterpret_cast<BYTE**>(&pColorBuffer));
           }
           else if (!color.empty())
           {
               nColorBufferSize = nColorWidth * nColorHeight * sizeof(RGBQUAD);
               hr = pColorFrame->CopyConvertedFrameDataToArray(nColorBufferSize, reinterpret_cast<BYTE*>(color.data), ColorImageFormat_Bgra);
               std::vector<cv::Mat> chanels,chanelsnew;
               cv::split(color,chanels);
              // chanels.erase(chanels.);
               chanelsnew.push_back(chanels[2]);
               chanelsnew.push_back(chanels[1]);
               chanelsnew.push_back(chanels[0]);
               cv::Mat color2;
               cv::merge(chanelsnew,color2);
               color = color2;
            /*   int j=0;
               for (int i=0;i<nColorWidth * nColorHeight;i++){
                   color.data[j]=pColorBuffer[i].rgbRed;
                   color.data[j+1]=pColorBuffer[i].rgbGreen;
                   color.data[j+2]=pColorBuffer[i].rgbBlue;
                   j=j+3;
               }*/
           }
           else
           {
               hr = E_FAIL;
           }
       }

       if (SUCCEEDED(hr))
       {
           depth = cv::Mat(nHeight,nWidth,CV_16UC1);
           memcpy(depth.data,pBuffer,nBufferSize*sizeof(UINT16));
       }

       SafeRelease(pColorFrameDescription);
       SafeRelease(pFrameDescription);
   }

   SafeRelease(pDepthFrame);
   SafeRelease(pColorFrame);
   SafeRelease(pMultiSourceFrame);

   if (SUCCEEDED(hr))
   {
       int F=120;
       float pixel_size=0.1042;

       RgbdKinect *kf = new RgbdKinect(pixel_size,F,"",color,depth,cv::Point3d(0,0,0));
       SEND(types::Rgbd,kf)
   }

   return esp();

}

void Kinect2SDK::setDeviceid(int id)
{
    SafeRelease(_pKinectSensor);
    HRESULT hr;

    hr = GetDefaultKinectSensor(&_pKinectSensor);
    if (FAILED(hr))
    {
         qDebug() << "Failed";
            return;
    }
    if (_pKinectSensor)
    {
            // Initialize the Kinect and get the depth reader
            //  IDepthFrameSource* pDepthFrameSource = NULL;
            hr = _pKinectSensor->Open();

            if (SUCCEEDED(hr))
            {
                _connected = true;
              //  hr = m_pKinectSensor->get_DepthFrameSource(&pDepthFrameSource);
                hr = _pKinectSensor->OpenMultiSourceFrameReader(FrameSourceTypes::FrameSourceTypes_Depth | FrameSourceTypes::FrameSourceTypes_Color ,
                    &_pMultiSourceFrameReader);
            }

            if (SUCCEEDED(hr))
            {
                ICoordinateMapper *_pMapper=0;
                hr = _pKinectSensor->get_CoordinateMapper(&_pMapper);
                if (SUCCEEDED(hr))
                {
                    CameraIntrinsics ci;
                    ci.FocalLengthX = 0;
                    while(0==ci.FocalLengthX){
                        Sleep(1500);
                        _pMapper->GetDepthCameraIntrinsics(&ci);
                    }
                }
            }

          /*  if (SUCCEEDED(hr))
            {
                hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
            }

            SafeRelease(pDepthFrameSource);*/
        }

        if (!_pKinectSensor || FAILED(hr))
        {
            _connected = false;
            return;
        }
}

Kinect2SDK::~Kinect2SDK()
{
    SafeRelease(_pMultiSourceFrameReader);
    // close the Kinect Sensor
    if (_pKinectSensor)
    {
        _pKinectSensor->Close();
    }
    SafeRelease(_pKinectSensor);
}

void Kinect2SDK::receiveProcess(esp data)
{
    get(data.getFrameId());
}

bool Kinect2SDK::getFrame(cv::Mat &mat)
{
    return false;
}

bool Kinect2SDK::isConnected()
{
    return _connected;
}
