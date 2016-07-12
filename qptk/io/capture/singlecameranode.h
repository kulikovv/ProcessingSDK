#ifndef PROCESSINGFRAMEWORK_SINGLECAMERANODE_H
#define PROCESSINGFRAMEWORK_SINGLECAMERANODE_H

#include <io/capture/cameranode.h>

namespace io
{
namespace capture{
class PROCESSINGFRAMEWORK_SHARED_EXPORT SingleCameraNode : public CameraNode
{
    Q_OBJECT
    Q_PROPERTY( int deviceid READ deviceid WRITE setDeviceid)
    Q_PROPERTY(bool isConnected READ isConnected)
public:
    explicit SingleCameraNode(QObject *parent = 0);
	~SingleCameraNode();
    int deviceid() const;
    virtual bool isConnected()=0;
public slots:
    virtual void setDeviceid (int value);

protected:
    virtual bool getFrame(cv::Mat &mat)=0;
    int _deviceid;
};
}
}

#endif // SINGLECAMERANODE_H
