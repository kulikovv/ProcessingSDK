#include "videosource.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <types/rgb.h>
#include <utils/logs.h>
#include <QThread>
#include <QtConcurrent/QtConcurrent>

using namespace io;
using namespace cv;
using namespace base;
using namespace io::media;
using namespace types;

VideoSource::VideoSource(QObject *parent) :
    SourceNode(parent)
{
    _last=0;
    _convert=false;
    _length = 0;
    _inlength = 0.0;
    _fps = 25;
    _cap = 0;
    _extension = "avi";
   // _cap = new cv::VideoCapture();
}
VideoSource::~VideoSource()
{
 //   _watcher.waitForFinished();
    if(0!=_cap)
    {
        delete _cap;
        _cap = 0;
    }
}

QIcon VideoSource::getIcon()
{
    return QIcon(":/sdk/io/media/VideoSource");
}

void VideoSource::checkVideoCapture(){
    if(!_cap)
    {
        _cap = new cv::VideoCapture();
    }
}

VideoSource::VideoSource(QString name,QObject *parent):
    SourceNode(parent)
{
    _last=0;
    _convert=false;
    _length = 0;
    _inlength = 0;
    _fps = 0;
    _cap = new cv::VideoCapture();
    setUrl(name);
}
/*int  VideoSource::readFrames()
{
    return _length;
}

void  VideoSource::setReadFrames(int rf)
{
    _length = rf;
}*/

void VideoSource::concurrentOpen(cv::VideoCapture* pCap,QString name)
{
     std::string copy = name.toStdString();
     pCap->open(copy);
}
void VideoSource::setUrl(QString value)
{
     _url = value;    
     checkVideoCapture();
     /*emit status(tr("Connecting..."),0,Node::Percent);
     connect(&_watcher,SIGNAL(finished()),this,SLOT(handleConnection()));
     QFuture<void> fopen = QtConcurrent::run(VideoSource::concurrentOpen,_cap, value);
     _watcher.setFuture(fopen);*/
     _cap->open(value.toStdString());
     handleConnection();

}

void VideoSource::handleConnection()
{
    if(_cap->isOpened())
    {
        //Считываеми параметры записи
        _inlength = _cap->get(CAP_PROP_FRAME_COUNT);
       setLength(_inlength);
       setFps(_cap->get(CAP_PROP_FPS));
       emit status(tr("Connected"),0,Node::Ready);
    }else{
        INFO("Capture not opened "+_url)
        emit status(tr("Error..."),0,Node::Error);
    }
}
int VideoSource::width()
{
    if(_cap)
    {
        if(_cap->isOpened())
        {
            return _cap->get(CAP_PROP_FRAME_WIDTH);
        }
    }
    return 0;
}

int VideoSource::height()
{
    if(_cap)
    {
        if(_cap->isOpened())
        {
            return _cap->get(CAP_PROP_FRAME_HEIGHT);
        }
    }
    return 0;
}

bool VideoSource::rgb2bgr()
{
    return _convert;
}

void VideoSource::setRgb2bgr(bool value)
{
    _convert = value;
}

esp VideoSource::get(const unsigned long frame)
{
    esp pointer;
    if(0==frame)
    {
        _cap->open(_url.toStdString());
    }

    Mat image;
    checkVideoCapture();
    //Если происходит смещение таймером
    if(-1 != (long int)frame && (frame - _last) > 1)
    {
        _cap->set(CAP_PROP_POS_FRAMES,frame);
       // double pos = (double)frame /(double) _inlength;
       // qDebug() << pos <<frame << _inlength;
       // _cap->set(CV_CAP_PROP_POS_AVI_RATIO,pos);
    }
    _cap->read(image);
    if(image.data)
    {
        //Переход к другой цветовой модели, если это надо
        if(_convert)
        {
            cv::cvtColor(image,image,COLOR_RGB2BGR);
        }
        SEND(Rgb,new Rgb(image));
	}else{
		INFO("No image data")
	}
    _last = frame;
    return pointer;
}

void VideoSource::receiveProcess(esp data)
{
    //TODO: reaction on reciving command
    get(data.getFrameId());
}

