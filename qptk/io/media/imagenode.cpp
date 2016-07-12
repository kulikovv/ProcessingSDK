#include "imagenode.h"
#include <base/esp.h>
#include <types/rgb.h>
#include <utils/logs.h>
#include <opencv2/highgui/highgui.hpp>


using namespace io;
using namespace io::media;
using namespace cv;
using namespace types;

ImageNode::ImageNode(QObject *parent) :
    SourceNode(parent)
{
    _fps = 0;

    _iscolor=1;
    _signs = 2;
    setMode(Write);
}

QIcon ImageNode::getIcon()
{
    return QIcon(":/sdk/io/media/ImageNode");
}

esp ImageNode::get(const unsigned long frame)
{
    esp pointer;
    QString final = _url;
    if(_url.contains("%1"))
    {
        int f = frame;
        final = _url.arg(f,_signs,10,QChar('0'));
    }

    Mat r = cv::imread(final.toStdString(),_iscolor);
    if(0!=r.data)
    {
        pointer = esp::init<Rgb>(new Rgb(r),frame);
        emit send(pointer);
    }else{
        INFO("Fail load image "+ final)
    }
    return pointer;
}

void ImageNode::receiveProcess(esp data)
{
    QSharedPointer<Rgb> mat = data.getReadOnly<Rgb>();
    if(0!=mat){
        cv::imwrite(_url.arg(data.getFrameId(),_signs,10,QChar('0')).toStdString(),mat->getMat());
    }else{
        get(data.getFrameId());
    }
}
void ImageNode::setUrl(QString value)
{
    _url = value;
}

ImageNode::Mode ImageNode::mode()
{
    return _mode;
}

bool ImageNode::isColorImage()
{
    return _iscolor;
}

void ImageNode::setColorImage(bool value)
{
    _iscolor = value;
}

void ImageNode::setMode(ImageNode::Mode value)
{
    _mode = value;
    switch(_mode)
    {
        case Read: setNodeType("Input");  _length=1;break;
        case Write: setNodeType("Output");  _length=0;break;
        case ReadWrite: setNodeType("Output"); break;
    }
}

int ImageNode::signs()
{
    return _signs;
}

void ImageNode::setSigns(int v)
{
    if(v>0){
        _signs = v;
    }
}
