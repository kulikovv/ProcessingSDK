#include "resizenode.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <types/rgb.h>

using namespace process;
using namespace types;

ResizeNode::ResizeNode(QObject *parent) :
    ProcessNode(parent)
{
    //TODO: Initiation source
    _width=320;
    _height=240;
}

QIcon ResizeNode::getIcon()
{
    return QIcon(":/sdk/process/ResizeNode");
}

ResizeNode::~ResizeNode()
{
    //TODO: Destruction	
}
int ResizeNode::width()
{
    return _width;
}

int ResizeNode::height()
{
    return _height;
}

void ResizeNode::setWidth(int value)
{
    _width = value;
}

void ResizeNode::setHeight(int value)
{
    _height = value;
}

void ResizeNode::receiveProcess(esp data)
{
    QSharedPointer<Rgb> mat = data.getReadOnly<Rgb>();
    if(!mat.isNull())
    {
        cv::Mat dst;
        cv::resize(mat->getMat(),dst,cv::Size(_width,_height));
        SEND(Rgb,new Rgb(dst))
    }
}

