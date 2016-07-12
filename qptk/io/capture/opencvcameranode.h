#ifndef PROCESSINGFRAMEWORK_OPENCVCAMERANODE_H
#define PROCESSINGFRAMEWORK_OPENCVCAMERANODE_H


#include <io/capture/singlecameranode.h>

namespace cv
{
    class VideoCapture;
}
namespace io
{
namespace capture{
class OpenCVCameraNode : public SingleCameraNode
{
    Q_OBJECT
public:
    explicit OpenCVCameraNode(QObject *parent = 0);
    ~OpenCVCameraNode();
    virtual esp get(const unsigned long frame);
    virtual bool isConnected();
signals:

public slots:
    virtual void setDeviceid (int value);
    virtual void setWidth(int value);
    virtual void setHeight(int value);
protected:
    virtual bool getFrame(cv::Mat &mat);

private:
    unsigned long posted;
    unsigned long _last;
protected:
    cv::VideoCapture* _cap;
};
}
}

#endif // OPENCVCAMERANODE_H
