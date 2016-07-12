#include "cameranode.h"

#include <QDebug>
#include <QDialog>
#include <utils/logs.h>

using namespace io;
using namespace io::capture;
using namespace camera;
CameraNode::CameraNode(QObject *parent) :
    SourceNode(parent)
{
    _width = 640;
    _height = 480;
    _fps = 30;


}
QIcon CameraNode::getIcon()
{
    return QIcon(":/sdk/io/capture/CameraNode");
}

CameraNode::~CameraNode()
{

}

void CameraNode::setWidth(int value)
{
    if(value>1)
    {
        this->_width = value;
    }else{
        INFO("Invalid Width")
    }
}

void CameraNode::setFps(double value)
{
    if(value>1)
    {
        SourceNode::setFps(value);
       // emit fpsChanged(_fps);
    }
}

void CameraNode::setHeight(int value)
{
    if(value>1)
    {
        this->_height = value;
    }else{
        INFO("Invalid Height")
    }
}

int CameraNode::width() const
{
    return _width;
}
int CameraNode::height() const
{
    return _height;
}
double CameraNode::fps() const
{
    return _fps;
}
void CameraNode::receiveProcess(esp data)
{
    get(data.getFrameId());
}

