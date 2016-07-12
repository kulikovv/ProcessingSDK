#ifndef PROCESSINGFRAMEWORK_VIDEOOUTPUT_H
#define PROCESSINGFRAMEWORK_VIDEOOUTPUT_H

#include <io/outputnode.h>
namespace types
{
    class Rgb;
}
namespace cv
{
    class VideoWriter;
}
namespace io{namespace media{

class PROCESSINGFRAMEWORK_SHARED_EXPORT VideoOutput : public OutputNode {
    Q_OBJECT
    Q_ENUMS(Codec)
    Q_PROPERTY(Codec compression READ compression WRITE setCompression)
    Q_PROPERTY(double fps READ fps WRITE setFps)
    Q_PROPERTY(int maxlength READ maxlength WRITE setMaxlength)

public:
    /*! Поведение узла в неактивном состоянии \sa enable
      */
    enum Codec {
        MPEG_1,
        motion_jpeg,
        MPEG_4_2,
        MPEG_4_3,
        MPEG_4,
        H263,
        H263I,
        XVID,
        FLV1,
        FFV1,
        NoCompress,
        SelectCodec
    };
    explicit VideoOutput(QObject *parent);
        ~VideoOutput();
    Codec compression() const;
    double fps() const;
    int maxlength() const;
public slots:
    virtual QIcon getIcon();
    void setFps(double value);
    virtual void start();
    /*! Окончить запись
    */
    virtual void stop();
    void setCompression(Codec value);
    void setMaxlength(int value);
protected:

    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
private:
    //Current data
   // QString _compression;
    cv::VideoWriter* writer;
    double _fps;
    int _maxlength;
    int _fourcc;
    Codec _codec;
    QString _innerurl;
};
}}

#endif // VIDEOOUTPUT_H


