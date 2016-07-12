#ifndef VIDEOINPUTCAPTURE_H
#define VIDEOINPUTCAPTURE_H

#include <io/capture/singlecameranode.h>

namespace io
{
namespace capture{
/*!
 * \brief The VideoInputCapture class захват видео с камеры с помощью библиотеки videoInput
 * Данный пакет работает только по операционной системой MS Windows
 */
class VideoInputCapture : public SingleCameraNode
{
    Q_OBJECT
public:
    explicit VideoInputCapture(QObject *parent = 0);
    virtual esp get(const unsigned long frame);
    virtual bool isConnected();
signals:

public slots:
    virtual void setDeviceid (int value);
protected:
    virtual bool getFrame(cv::Mat &mat);
    
};
}
}

#endif // VIDEOINPUTCAPTURE_H
