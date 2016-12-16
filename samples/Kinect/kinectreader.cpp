#include "kinectreader.h"
#include <QDebug>
#include <types/rgbd.h>
#include <iostream>
#include "rgbdkinect.h"
#include <opencv2/imgproc/imgproc.hpp>
KinectReader::KinectReader(QObject *parent) :
    io::SourceNode(parent)
{
    _isopen = false;
    _pixelsize=0.1042f;
    _extension = "mkv";
    _F=120.f;
    setUrl("E:/xxx.mkv");
    _last = 0;
}

QIcon KinectReader::getIcon()
{
    return QIcon(":/kinectlibrary/KinectReader");
}


esp KinectReader::get(const base::timestamp frame)
{
    video_io::VideoReader::ReadParams readParams;
   // readParams.fixAspectRatio = true;

    if (_isopen)
    {
        cv::Mat rgb;
        cv::Mat depth;

        if(-1 != (long int)frame && fabs(double(frame - _last)) > 1)
        {
        //    qDebug()<<"Seek to xxx"<<frame;
            _videoReaderColor.seek(frame);
            _videoReaderDepth.seek(frame);
        }

        int id = _videoReaderColor.read(rgb,0, readParams);
        int id2 = _videoReaderDepth.read(depth,1, readParams);
      //  qDebug() << depth.channels() << (CV_16U==depth.depth()) << depth.rows << depth.cols;



        if (id < 0||id2 < 0)
        {

            qDebug() << "Error";
            //_isopen = false;
            emit status("stop",0,Node::Ready);
            //end
        }else{
           // qDebug() << "_pixelsize" << _pixelsize << "F" << _F;
            cv::Mat rgbFull;
            cv::resize(rgb,rgbFull,cv::Size(640,480));
            SEND(types::Rgbd,new RgbdKinect(_pixelsize,_F,"",rgbFull,depth.clone(),cv::Point3d(0,0,0)))
        }

        _last = frame;

    }
    return esp();
}

void printVideoReaderInfo(const char *fileName, video_io::VideoReader const &videoReader)
{
    std::cout << "**************************************************" << std::endl;
    std::cout << std::endl;

    std::cout << "fileName: " << fileName << std::endl;
    std::cout << std::endl;

    std::cout << "nbStreams              : " << videoReader.nbStreams() << std::endl;
    std::cout << "nbVideoStreams         : " << videoReader.nbVideoStreams() << std::endl;
    std::cout << "nbReadableVideoStreams : " << videoReader.nbReadableVideoStreams() << std::endl;
    std::cout << std::endl;

    video_io::Metadata metadata;
  //  std::string x = metadata["xxx"];

    videoReader.getMetadata(metadata);

    if (!metadata.empty())
    {
        video_io::Metadata::const_iterator it;

        std::cout << "Metadata" << std::endl;
        std::cout << std::endl;

        for (it = metadata.begin(); it != metadata.end(); ++it)
            std::cout << "  " << it->first << " : " << it->second << std::endl;

        std::cout << std::endl;
    }

    std::cout << "Stream info" << std::endl;
    std::cout << std::endl;

    for (int id = 0; id < videoReader.nbStreams(); ++id)
    {
        std::cout << "  stream id: " << id << std::endl;
        std::cout << "    nbThreads        : " << videoReader.nbThreads(id) << std::endl;
        std::cout << "    codecDescription : " << videoReader.codecDescription(id) << std::endl;
        std::cout << "    codecName        : " << videoReader.codecName(id) << std::endl;
        std::cout << "    fourCC           : " << videoReader.fourCC(id) << std::endl;
        std::cout << "    isVideoStream    : " << videoReader.isVideoStream(id) << std::endl;
        std::cout << "    nbFrames         : " << videoReader.nbFrames(id) << std::endl;
        std::cout << "    duration         : " << videoReader.duration(id) << std::endl;
        std::cout << "    frameRate        : " << videoReader.frameRate(id) << std::endl;
        std::cout << "    avgFrameRate     : " << videoReader.avgFrameRate(id) << std::endl;
        std::cout << "    pixelFormat      : " << videoReader.pixelFormat(id) << std::endl;
        std::cout << "    width            : " << videoReader.width(id) << std::endl;
        std::cout << "    height           : " << videoReader.height(id) << std::endl;
        std::cout << "    aspectRatio      : " << videoReader.aspectRatio(id) << std::endl;
        std::cout << "    bitRate          : " << videoReader.bitRate(id) << std::endl;
        std::cout << "    bitRateTolerance : " << videoReader.bitRateTolerance(id) << std::endl;
        std::cout << "    gopSize          : " << videoReader.gopSize(id) << std::endl;
        std::cout << "    maxBFrames       : " << videoReader.maxBFrames(id) << std::endl;
        std::cout << std::endl;
    }
}

void KinectReader::setUrl(QString value)
{

    try{
        if(_isopen)
        {
            _videoReaderColor.close();
            _videoReaderDepth.close();
            _isopen = false;
        }
        video_io::Metadata mdata;
        _videoReaderColor.open(value.toStdString());
        printVideoReaderInfo("Ololo",_videoReaderColor);
        _videoReaderDepth.open(value.toStdString());
        if (_videoReaderColor.nbReadableVideoStreams() > 0)
        {
            _videoReaderColor.getMetadata(mdata);
            video_io::Metadata::const_iterator it;
            for(it=mdata.begin();it!=mdata.end();++it)
            {
                qDebug() << QString::fromStdString(it->first) << QString::fromStdString(it->second);
            }
            QString F = QString::fromStdString(mdata["ZPD"]);
            if(!F.isEmpty())
            {
                _F = F.toDouble();
            }
            QString PX = QString::fromStdString(mdata["ZPPS"]);
            if(!PX.isEmpty())
            {
                _pixelsize = PX.toDouble();
            }

          //  qDebug() << "NumbFr" << _videoReaderColor.duration(1);
         //   qDebug() << "NumbFr" << _videoReaderDepth.duration(5)<<_videoReaderDepth.nbFrames(5);
            setFps(_videoReaderColor.frameRate(0));
            setLength(_videoReaderColor.nbFrames(0));
            _isopen = true;
            io::SourceNode::setUrl(value);
            return;
        }
    }
    catch (video_io::VideoReader::Error const &e)
    {
        qDebug() << "VideoReader error: " << e.what();

        return;
    }
    catch (video_io::VideoWriter::Error const &e)
    {
        qDebug() <<"VideoWriter error: " << e.what();

        return;
    }
    catch (std::exception const &e)
    {
        qDebug() << "Error: " << e.what();

        return;
    }
    _isopen = false;

}
