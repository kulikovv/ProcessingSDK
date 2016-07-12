#include "videooutput.h"
#include <types/rgb.h>
#include <opencv2/highgui/highgui.hpp>
#include <QTimer>
#include <utils/logs.h>
#include <cstdint>
using namespace io;
using namespace cv;
using namespace types;
using namespace io::media;

VideoOutput::VideoOutput(QObject *parent) :
    OutputNode(parent)
{
    //TODO: Initiation source
    _url = "";
    _innerurl = "";
    _extension = "avi";
//    _compression = "XVID";
    _fps = 25.f;
    writer = 0;//new cv::VideoWriter();
    _maxlength = 0;
    _codec = io::media::VideoOutput::MPEG_4;
   // _fourcc = CV_FOURCC('D','I','V','X');
    setCompression(_codec);
}
VideoOutput::~VideoOutput()
{
	stop();
}

QIcon VideoOutput::getIcon()
{
    return QIcon(":/sdk/io/media/VideoOutput");
}
//This function is called if this node is enable
void VideoOutput::receiveProcess(esp data)
{
    QSharedPointer<Rgb> var = data.getReadOnly<Rgb>();
    if(0!=var)
    {
        if(_innerurl!=_url)
        {
            if(writer)
            {
                delete writer;
                writer = 0;
            }
        }
        if(0==writer)
        {
            qDebug() << "URL:"<<_url << var->getMat().cols << var->getMat().rows;
            if(!_url.isEmpty())
            {
                writer = new cv::VideoWriter();
               // cv::VideoWriter ppp;
                /*_url.toStdString(),_fourcc,_fps,
                            Size(var->getMat().cols,var->getMat().rows),
                            var->getMat().channels()==1?false:true);*/

                //writer.open("D:/test.avi",CV_FOURCC('M','P','E','G'),25,cv::Size(img.cols,img.rows),true);
                  //

                if(!writer->open(_url.toStdString(),_fourcc,_fps,Size(var->getMat().cols,var->getMat().rows),var->getMat().channels()==1?false:true))
                {
                    qDebug() << "video output not opened";
                }
                _innerurl=_url;

            }else{
                return;
            }
        }
        if(writer->isOpened())
        {
            qDebug() << "Write";
            writer->write(var->getMat());
        }
    }
}
double VideoOutput::fps() const
{
    return _fps;
}

int VideoOutput::maxlength() const
{
    return _maxlength;
}

void VideoOutput::setMaxlength(int value)
{
    _maxlength = value;
}

void VideoOutput::setFps(double value)
{
    _fps = value;
}

VideoOutput::Codec VideoOutput::compression() const
{
    return _codec;
}

uint32_t FOURCC(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
     return ( (uint32_t) (((d)<<24) | (uint32_t(c)<<16) | (uint32_t(b)<<8) | uint32_t(a)) );
}

void VideoOutput::setCompression(VideoOutput::Codec codec)
{
  /*  if(4==value.length())
    {
        _compression = value;
    }*/
    _codec = codec;
    switch(_codec)
    {
        case MPEG_1:        _fourcc =FOURCC('P','I','M','1'); break;
        case motion_jpeg:   _fourcc =FOURCC('M','J','P','G'); break;
        case MPEG_4_2:      _fourcc =FOURCC('M','P','4','2'); break;
        case MPEG_4_3:      _fourcc =FOURCC('D','I','V','3'); break;
        case MPEG_4:        _fourcc =FOURCC('D','I','V','X'); break;
        case H263:          _fourcc =FOURCC('U','2','6','3'); break;
        case H263I:         _fourcc =FOURCC('I','2','6','3'); break;
        case XVID:          _fourcc =FOURCC('X','V','I','D'); break;
        case FLV1:          _fourcc =FOURCC('F','L','V','1'); break;
        case FFV1:          _fourcc =FOURCC('F','F','V','1'); break;
        case NoCompress:    _fourcc =FOURCC('H','F','Y','U'); break;
        case SelectCodec:   _fourcc = -1;
        default:            _fourcc =FOURCC('D','I','V','X'); break;
    }

    /*
    CV_FOURCC('P','I','M','1') // MPEG-1 codec

    CV_FOURCC('M','J','P','G') // motion-jpeg codec (does not work well)

    CV_FOURCC('M','P','4','2') // MPEG-4.2 codec

    CV_FOURCC('D','I','V','3') // MPEG-4.3 codec

    CV_FOURCC('D','I','V','X') // MPEG-4 codec

    CV_FOURCC('U','2','6','3') // H263 codec

    CV_FOURCC('I','2','6','3') // H263I codec

    CV_FOURCC('X','V','I','D') // XVID codec

    CV_FOURCC('F','L','V','1') // FLV1 codec */
}

void VideoOutput::start()
{
    if(_maxlength>0){
        INFO("Timer set")
        QTimer *timer = new QTimer(this);
        timer->setSingleShot(true);
        timer->setInterval(_maxlength);
        connect(timer,SIGNAL(timeout()),this,SLOT(stop()));
        timer->start();
    }
}

/*! Окончить запись
*/
void VideoOutput::stop()
{
    INFO("Timer worked")
    if(0!=writer)
    {
        delete writer;
        writer = 0;
    }
    emit ended(_url);
    OutputNode::stop();
}
