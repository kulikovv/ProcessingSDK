#include "kinectwriter.h"
#include <types/rgbd.h>
#include <QDebug>
#include <QFile>
#include <utils/logs.h>
#include <opencv2/imgproc/imgproc.hpp>

KinectWriter::KinectWriter(QObject *parent) :
    io::OutputNode(parent)
{
    INFO("CREATING")
    videoWriter = new video_io::VideoWriter();
    _isopen = false;
    _writedMetadata=false;
    _extension = "mkv";
    _doresize = true;
    _fps = 24;
    _counter=0;
   // setUrl("E:/xxx.mkv");
}

QIcon KinectWriter::getIcon()
{
    return QIcon(":/kinectlibrary/KinectWriter");
}

KinectWriter::~KinectWriter()
{
    stop();
}

void KinectWriter::receiveProcess(esp data)
{
    QSharedPointer<types::Rgbd> readonly = data.getReadOnly<types::Rgbd>();
    if(0!=readonly&&_isopen)
    {
        if(!_writedMetadata)
        {
            // Добавляем метаданные.
            //video_io::Metadata metadata;
           // metadata.insert(video_io::Metadata::value_type("Kinect", "f: 1000, k: 1, m0: (255, 270), gamma: 1"));
            videoWriter->setMetadata(readonly->getConfig());
            _writedMetadata = true;
        }

        cv::Mat rgbCutted;
        if(_doresize){
            cv::resize(readonly->rgb().getMat(),rgbCutted,cv::Size(320,240));
        }else{
            rgbCutted = readonly->rgb().getMat();
        }

      //  qDebug() << "Writed:" << _counter++ << "Current:" << frame;
        videoWriter->write(rgbCutted, 0);
        videoWriter->write(readonly->depth().getMat(), 1);

    }

}

void KinectWriter::stop()
{
    if(_isopen&&videoWriter)
    {
        videoWriter->close();
        delete videoWriter;
        videoWriter = new video_io::VideoWriter();
        _isopen = false;
    }
}

void KinectWriter::setResizeRgb(bool resize)
{
    _doresize = resize;
}

void KinectWriter::setVideoFps(int fps)
{
    if(fps>1&&fps<30){
        _fps = fps;
    }
}

void KinectWriter::setUrl(QString value)
{
    INFO("Try open file "+value)
    if(QFile::exists(value))
    {
        CRITICAL("File already exists no rewrite allow");
        return;
    }
    const char *outputPixelFormats[] = {
        "yuv420p",
        "gray16"

    };

    const char *outputCodecNames[] = {
        "wmv2",
        "ffv1"
    };

    int szx[] = {
        320,
        640
    };
    int szy[] = {
        240,
        480
    };

    if(!_doresize){
        szx[0] = 640;
        szy[0] = 480;
    }


    if(_isopen)
    {
        _writedMetadata=false;
        videoWriter->close();
        delete videoWriter;
        videoWriter = new video_io::VideoWriter();
        _isopen = false;

    }

    if(value.isEmpty())
    {
        return;
    }

    try{
        io::OutputNode::setUrl(value);
        videoWriter->open(value.toStdString());
        _isopen = true;

        // Создаем в выходном файле видеопотоки.

        for (int n = 0; n < 2; ++n)
        {
            try{
            video_io::VideoWriter::VideoStreamParams videoStreamParams;

            videoStreamParams.codecName = outputCodecNames[n];
            videoStreamParams.pixelFormat = outputPixelFormats[n];
            videoStreamParams.frameRate = _fps;
            videoStreamParams.width = szx[n];
            videoStreamParams.height = szy[n];
            videoStreamParams.findBestPixelFormat = true;

            videoWriter->addVideoStream(videoStreamParams);
            }catch(std::exception ex)
            {
                 qDebug() << outputCodecNames[n] << "dont support" << ex.what();
            }
        }
    }catch(std::exception ex)
    {
        if(_isopen)
        {
            videoWriter->close();
        }
        qDebug() << ex.what();
    }catch(...)
    {
        if(_isopen)
        {
            videoWriter->close();
        }
        qDebug() << "unknown error";
    }
    _counter=0;

}
