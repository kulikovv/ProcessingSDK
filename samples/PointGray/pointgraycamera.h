#ifndef POINTGRAYCAMERA_H
#define POINTGRAYCAMERA_H

#include <io/capture/cameranode.h>

#include "FlyCapture2.h"
/*
namespace FlyCapture2{
class Error;
class Camera;
enum VideoMode;
}*/

#define CHECK_ERROR(x)  if (x != PGRERROR_OK)\
{\
    qDebug() << "Error" ;\
    error.PrintErrorTrace();\
    return;\
}

#define MAX_CAMS 5

typedef FlyCapture2::FrameRate MyFR;
using namespace FlyCapture2;
class PointGrayCamera : public io::capture::CameraNode
{
    Q_OBJECT

    Q_ENUMS(VideoModeX)
    Q_ENUMS(FrameRateX)
    Q_PROPERTY( double fps READ fps WRITE setFps)
    Q_PROPERTY(QString serial READ serial)
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(VideoModeX videoMode READ videoMode WRITE setVideoMode)
    Q_PROPERTY(FrameRateX frameRate READ frameRate WRITE setFrameRate)
    Q_PROPERTY(int numCam READ numCam)
    Q_PROPERTY(double shutter READ shutter WRITE setShutter)
public:

   // enum MyFR;

    enum FrameRateX
    {
        FRAMERATE_1_875, /**< 1.875 fps. */
        FRAMERATE_3_75, /**< 3.75 fps. */
        FRAMERATE_7_5, /**< 7.5 fps. */
        FRAMERATE_15, /**< 15 fps. */
        FRAMERATE_30, /**< 30 fps. */
        FRAMERATE_60, /**< 60 fps. */
        FRAMERATE_120, /**< 120 fps. */
        FRAMERATE_240, /**< 240 fps. */
        FRAMERATE_FORMAT7, /**< Custom frame rate for Format7 functionality. */
        NUM_FRAMERATES, /**< Number of possible camera frame rates. */
        FRAMERATE_FORCE_32BITS = FULL_32BIT_VALUE

    };

    /** DCAM video modes. */
    enum VideoModeX
    {
        VIDEOMODE_160x120YUV444, /**< 160x120 YUV444. */
        VIDEOMODE_320x240YUV422, /**< 320x240 YUV422. */
        VIDEOMODE_640x480YUV411, /**< 640x480 YUV411. */
        VIDEOMODE_640x480YUV422, /**< 640x480 YUV422. */
        VIDEOMODE_640x480RGB, /**< 640x480 24-bit RGB. */
        VIDEOMODE_640x480Y8, /**< 640x480 8-bit. */
        VIDEOMODE_640x480Y16, /**< 640x480 16-bit. */
        VIDEOMODE_800x600YUV422, /**< 800x600 YUV422. */
        VIDEOMODE_800x600RGB, /**< 800x600 RGB. */
        VIDEOMODE_800x600Y8, /**< 800x600 8-bit. */
        VIDEOMODE_800x600Y16, /**< 800x600 16-bit. */
        VIDEOMODE_1024x768YUV422, /**< 1024x768 YUV422. */
        VIDEOMODE_1024x768RGB, /**< 1024x768 RGB. */
        VIDEOMODE_1024x768Y8, /**< 1024x768 8-bit. */
        VIDEOMODE_1024x768Y16, /**< 1024x768 16-bit. */
        VIDEOMODE_1280x960YUV422, /**< 1280x960 YUV422. */
        VIDEOMODE_1280x960RGB, /**< 1280x960 RGB. */
        VIDEOMODE_1280x960Y8, /**< 1280x960 8-bit. */
        VIDEOMODE_1280x960Y16, /**< 1280x960 16-bit. */
        VIDEOMODE_1600x1200YUV422, /**< 1600x1200 YUV422. */
        VIDEOMODE_1600x1200RGB, /**< 1600x1200 RGB. */
        VIDEOMODE_1600x1200Y8, /**< 1600x1200 8-bit. */
        VIDEOMODE_1600x1200Y16, /**< 1600x1200 16-bit. */
        VIDEOMODE_FORMAT7, /**< Custom video mode for Format7 functionality. */
        NUM_VIDEOMODES, /**< Number of possible video modes. */
        VIDEOMODE_FORCE_32BITS = FULL_32BIT_VALUE
    };


    explicit PointGrayCamera(QObject *parent = 0);
    ~PointGrayCamera();
    /*! Аналог preloadframe, при вызове метода загружается изображение
      * и отправляется сигналом send
      * \param frame номер кадра который хотим загрузить
      * \return умный указатель на отправленные данные
      */
    virtual esp get(const base::timestamp frame);
    /*!
     * \brief serial возвращает серийный номер камеры
     * \return возвращает серийный номер камеры
     */
    QString serial() const;
    /*!
     * \brief id установленный идентификатор камеры в системе.
     * \warning При переподключениях камер идентификаторы могуть меняться
     * \return
     */
    int id() const;
    VideoModeX videoMode() const;
    FrameRateX frameRate() const;
    int numCam() const;
    double shutter();
    double fps();
signals:
    
public slots:
    void setShutter(double value);
    void setId(const int id);
    void setVideoMode(const VideoModeX vm );
    void setFrameRate(const FrameRateX fr);
    void setFps(double value);
    void getFrame();
protected:
    FlyCapture2::Property buildProperty(PropertyType pt, float value, bool autoValue=false);
    void PrintError(FlyCapture2::Error error );
    FlyCapture2::Camera* _cam;
    void videoMode2WH(FlyCapture2::VideoMode vm);
    void frameRate2Fps(FlyCapture2::FrameRate fr);
    FlyCapture2::VideoMode _vm;
    FlyCapture2::FrameRate _fr;
    QString _serial;
    int _id;
    static int _ncams;
    static QList<int> _usedCams;
    QElapsedTimer timerx;
    double _lasttimestamp;
    cv::Mat _image;
   // unsigned char* _secretdata;
private:
    static void callBack(FlyCapture2::Image *pImage, const void *pCallbackData) ;
    static cv::Mat* _mats[MAX_CAMS];
   // static FlyCapture2::Image* _ptr[MAX_CAMS];
   // static QMutex _mutex[MAX_CAMS];
    unsigned int _imsize2;
};

#endif // POINTGRAYCAMERA_H
