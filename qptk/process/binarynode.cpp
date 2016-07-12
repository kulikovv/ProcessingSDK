#include "binarynode.h"
#include <types/rgb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <types/callbackhook.h>
#include <utils/logs.h>

using namespace process;
using namespace base;
using namespace types;

BinaryNode::BinaryNode(QObject *parent) :
    ProcessNode(parent)
{
    //TODO: Initiation source
    this->_maskUrl = "";
    _inverted = false;
    _threshold = -1;
    _adaptiveSize = 20;
    _adaptive = false;
}

QIcon BinaryNode::getIcon()
{
    return QIcon(":/sdk/process/BinaryNode");
}

QString BinaryNode::maskUrl()
{
    return _maskUrl;
}


void BinaryNode::setMaskUrl(QString str)
{
    _mask = cv::imread(str.toStdString());
}

void BinaryNode::receiveProcess(esp data)
{
    if(data=="HOOK")
    {
        emit send(data);
        return;
    }


    QSharedPointer<Rgb> readonly = data.getReadOnly<Rgb>();
    if(data=="MASK"&&!readonly.isNull())
    {
        _mask = readonly->getMat();
        if(1!=_mask.channels())
        {
            cv::Mat graymask;
            cv::cvtColor(_mask,graymask,cv::COLOR_RGB2GRAY);
            _mask = graymask;
        }
        _mask=255-_mask;
        emit send(data);
        //SEND(Rgb,new Rgb(grey))
        return;
    }

    if(!readonly.isNull())
    {

        if(3==readonly->getMat().channels())
        {
            cv::cvtColor(readonly->getMat(),grey,cv::COLOR_RGB2GRAY);
        }else{
            grey = readonly->getMat().clone();
        }

        int flags = 0;
        int t=0;
        if(-1==_threshold){
             flags =flags|cv::THRESH_OTSU;
        }else{
            t=_threshold;
            flags =flags|cv::THRESH_BINARY;
        }
        if(_inverted)
        {
            flags =flags|cv::THRESH_BINARY_INV;
        }else{
            flags =flags|cv::THRESH_BINARY;

        }

        if(!_adaptive)
        {
            cv::threshold(grey,grey, t, 255, flags);
        }else{
            cv::adaptiveThreshold(grey,grey,255,cv::ADAPTIVE_THRESH_MEAN_C,flags,_adaptiveSize,t);
        }
        if(_mask.cols==grey.cols&&_mask.rows==grey.rows)
        {
            grey.setTo(cv::Scalar(0),_mask);
        }

        SEND(Rgb,new Rgb(grey))
    }
}
int BinaryNode::threshold()
{
    return _threshold;
}

void BinaryNode::setThreshold(int value)
{
    _threshold = value;
}

bool BinaryNode::isInverted()
{
    return _inverted;
}

void BinaryNode::setInverted(bool value)
{
    _inverted = value;
}

