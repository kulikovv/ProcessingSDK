#include "singlecameranode.h"
#include <camera/calibration.h>

using namespace camera;
using namespace io;
using namespace io::capture;

SingleCameraNode::SingleCameraNode(QObject *parent) :
    CameraNode(parent)
{
    _deviceid = 0;

}
SingleCameraNode::~SingleCameraNode()
{

}
int SingleCameraNode::deviceid() const
{
    return this->_deviceid;
}
void SingleCameraNode::setDeviceid (int value)
{
    if(_deviceid>0)
    {
        _deviceid = value;
    }
}

