#ifndef KINECTWRITER_H
#define KINECTWRITER_H

#include <io/outputnode.h>
#include <VideoIO/VideoIO.h>
#include "kinectlibrary_global.h"

class KINECTLIBRARYSHARED_EXPORT KinectWriter : public io::OutputNode
{
    Q_OBJECT
    Q_PROPERTY(bool ResizeRgb READ resizeRgb WRITE setResizeRgb )
    Q_PROPERTY(int videoFps READ videoFps WRITE setVideoFps )
public:
    explicit KinectWriter(QObject *parent = 0);
    ~KinectWriter();

    bool resizeRgb(){
        return _doresize;
    }

    int videoFps(){
        return _fps;
    }

signals:
    
public slots:
    virtual QIcon getIcon();
    virtual void stop();
    virtual void setUrl(QString value);
    void setResizeRgb(bool resize);
    void setVideoFps(int fps);
protected:
    /*! Переопределяемая функция обработки данных
      * т.к. скорость вызова виртуальной функции >> чем вызов виртуального СЛОТа то
      * мы ее выделили
      */
    virtual void receiveProcess(esp data);
private:
    video_io::VideoWriter* videoWriter;
    bool _isopen;
    bool _writedMetadata;
    bool _doresize;
    int _fps;
    int _counter;
};

#endif // KINECTWRITER_H
