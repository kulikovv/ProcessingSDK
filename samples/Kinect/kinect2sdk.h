#ifndef KINECT2SDK_H
#define KINECT2SDK_H

#include <QMutex>

#include "sal.h"
#include <Kinect.h>
#include <io/sourcenode.h>
#include <io/capture/singlecameranode.h>
#include <opencv2/core/core.hpp>

#include "kinectlibrary_global.h"
using namespace io;

/*!
 * \brief The Kinect2SDK class работает с Kinect 2 SDK
 * загружает параметры и сохраняет их в формат Rgbd видимый EthoStudio и другими приложения
 */
class KINECTLIBRARYSHARED_EXPORT Kinect2SDK : public capture::SingleCameraNode
{
    Q_OBJECT
public:
    explicit Kinect2SDK(QObject *parent);
    ~Kinect2SDK();

    virtual esp get(const base::timestamp frame);
    virtual bool isConnected();
public slots:
    virtual QIcon getIcon();
    void setDeviceid(int id);
protected:
    void disconnect();
    virtual void receiveProcess(esp data);
    virtual bool getFrame(cv::Mat &mat);
    bool _connected;
    //Sensor iterface
    IKinectSensor*          _pKinectSensor;
    // Depth reader
    // IDepthFrameReader*       m_pDepthFrameReader=0;
    // Frame reader
    IMultiSourceFrameReader* _pMultiSourceFrameReader;
};

#endif // KINECT2SDK_H
