#ifndef KINECTREADER_H
#define KINECTREADER_H

#include <io/sourcenode.h>
#include <VideoIO/VideoIO.h>
#include "kinectlibrary_global.h"

class KINECTLIBRARYSHARED_EXPORT KinectReader : public io::SourceNode
{
    Q_OBJECT
public:
    explicit KinectReader(QObject *parent = 0);
    virtual esp get(const base::timestamp frame);
signals:
    
public slots:
    virtual QIcon getIcon();
    virtual void setUrl(QString value);
protected:
    video_io::VideoReader _videoReaderColor;
    video_io::VideoReader _videoReaderDepth;
    bool _isopen;
    double _pixelsize;
    double _F;
    int _last;
};

#endif // KINECTREADER_H
