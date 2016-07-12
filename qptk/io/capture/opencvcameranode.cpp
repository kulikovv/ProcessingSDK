#include "opencvcameranode.h"
#include <utils/logs.h>
#include <types/rgb.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <camera/calibration.h>

using namespace io;
using namespace io::capture;
using namespace types;

OpenCVCameraNode::OpenCVCameraNode(QObject *parent) :
    SingleCameraNode(parent)
{
    _cap=0;
}
bool OpenCVCameraNode::isConnected()
{
    if(_cap)
    {
        return _cap->isOpened();
    }
    return false;
}

OpenCVCameraNode::~OpenCVCameraNode()
{
    if(_cap)
    {
		_cap->release();
        delete _cap;
        _cap=0;
    }
}
void OpenCVCameraNode::setWidth(int value)
{
    if(_cap)
    {
        _cap->set(cv::CAP_PROP_FRAME_WIDTH,value);
    }
   _width = value;
}

void OpenCVCameraNode::setHeight(int value)
{
    if(_cap)
    {
        _cap->set(cv::CAP_PROP_FRAME_HEIGHT,value);
    }
     _height = value;
}

void OpenCVCameraNode::setDeviceid(int value)
{
    if(_cap)
    {
		_cap->release();
        delete _cap;
        _cap = 0;
    }

    if(value<0)
    {
        return;
    }

    posted = 0;
    qDebug() << "Create capture";
    _cap = new cv::VideoCapture(value);
     qDebug() << "Capture creted";
  //  _cap->set(CV_CAP_PROP_FRAME_WIDTH,this->width());
  //  _cap->set(CV_CAP_PROP_FRAME_HEIGHT,this->height());
    if( _cap->isOpened())
    {
    //    setLength(_cap->get(CV_CAP_PROP_FRAME_COUNT));
     //   setFps(_cap->get(CV_CAP_PROP_FPS));
      //  setWidth(_cap->get(CV_CAP_PROP_FRAME_WIDTH));
      //  setHeight(_cap->get(CV_CAP_PROP_FRAME_HEIGHT));
        SingleCameraNode::setDeviceid(value);
    }else{

        INFO("Capture not opened. Invalid device id "+QString("%1").arg(value));
    }
	_deviceid = value;
}

esp OpenCVCameraNode::get(const unsigned long frame)
{
  //  qDebug() << "get toc count" << this->tok(frame);
    esp pointer;
    if(!_cap)
        return pointer;
    cv::Mat image;
    if(!getFrame(image))
    {
        return pointer;
    }

    if(image.data)
    {
        SEND(Rgb, new Rgb(image))
    }
    posted++;
  //  qDebug() << "get toc count" << this->tok(frame);
    return pointer;
}

bool OpenCVCameraNode::getFrame(cv::Mat &mat)
{
    if(!_cap)
        return false;


    if(_cap->isOpened())
    {
        if(_cap->grab())
        {
            if(!_cap->retrieve(mat))
            {
                CRITICAL("Can't retrive")
            }
        }else{
            CRITICAL("Can't grab")
        }
        // _cap->read(mat);

         if(mat.empty())
         {
             CRITICAL("Frame is empty")
             return false;
         }
          cv::cvtColor(mat,mat,cv::COLOR_RGB2BGR);


        return true;
    }
    return false;
}

