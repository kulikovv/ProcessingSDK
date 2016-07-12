#ifndef PROCESSINGFRAMEWORK_STERIOCAMERANODE_H
#define PROCESSINGFRAMEWORK_STERIOCAMERANODE_H

#include <io/capture/cameranode.h>

namespace io
{
namespace capture{
class PROCESSINGFRAMEWORK_SHARED_EXPORT StereoCameraNode : public CameraNode
{
    Q_OBJECT
public:
    explicit StereoCameraNode(QObject *parent = 0);
    cv::Mat joinPics(cv::Mat left,cv::Mat right);
public slots:
    virtual void frameRequest();
protected:
    virtual bool getFrame(cv::Mat &left,cv::Mat &right)=0;
};
}
}

#endif // STERIOCAMERANODE_H
