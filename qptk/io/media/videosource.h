#ifndef PROCESSINGFRAMEWORK_VIDEOSOURCE_H
#define PROCESSINGFRAMEWORK_VIDEOSOURCE_H

#include <io/sourcenode.h>

#include <QFutureWatcher>
/*! \brief Пространство имен из OpenCV
  Пространство имен из библиотеки компьютерного зрения OpenCV
  */
namespace cv
{
    class VideoCapture;
}
namespace io
{
    namespace media
    {

class PROCESSINGFRAMEWORK_SHARED_EXPORT VideoSource : public SourceNode {
    Q_OBJECT
  //  Q_PROPERTY( int readFrames READ readFrames WRITE setReadFrames )
    Q_PROPERTY( bool rgb2bgr READ rgb2bgr WRITE setRgb2bgr )
    Q_PROPERTY(int width READ width)
    Q_PROPERTY(int height READ height)
    Q_CLASSINFO("Send","PMat")
public:
    explicit VideoSource(QObject *parent);
	~VideoSource();
    explicit VideoSource(QString name,QObject *parent);
    /*!Call from synchronizer 
      * general rule frame = (0,length())
      * if frame = -1 it means to take next frame
      */
    virtual esp get(const unsigned long frame);
  //  int readFrames();
    bool rgb2bgr();
    int width();
    int height();
public slots:
    virtual QIcon getIcon();
  //  void setReadFrames(int rf);
    virtual void setUrl(QString value);
    void setRgb2bgr(bool value);
protected:
    /*! Call from other Nodes
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
    void checkVideoCapture();
    static void concurrentOpen(cv::VideoCapture* pCap,QString name);
protected slots:
    void handleConnection();
private:
    unsigned long _last;
    cv::VideoCapture* _cap;
    bool _convert;
    QFutureWatcher<void> _watcher;
    double _inlength;

};
}
}
#endif // VIDEOSOURCE_H


