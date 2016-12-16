#ifndef KINECTOPENNI_H
#define KINECTOPENNI_H 
#include <OpenNI.h>
#include <QMutex>
#include <io/sourcenode.h>
#include <io/capture/singlecameranode.h>
#include <vector>
#include <opencv2/core/core.hpp>

#include "kinectlibrary_global.h"
using namespace io;

class KINECTLIBRARYSHARED_EXPORT KinectOpenNI : public capture::SingleCameraNode
{
	Q_OBJECT

   // Q_PROPERTY(bool sendKinectFrame READ sendKinectFrame WRITE setSendKinectFrame)
   // Q_PROPERTY(bool sendPointCloud READ sendPointCloud WRITE setSendPointCloud)
   // Q_PROPERTY(bool send3D READ send3D WRITE setSend3D)
    Q_PROPERTY(QString Serial READ serial WRITE setSerial)
    Q_PROPERTY(bool flip READ flip WRITE setflip)
	Q_CLASSINFO("Send", "KinectFrame")
public:
	explicit KinectOpenNI(QObject *parent);
	~KinectOpenNI();

    virtual esp get(const base::timestamp frame);
    virtual bool isConnected();
    bool flip();
public slots:
    virtual QIcon getIcon();
	QString serial();
     //   bool sendKinectFrame();
     ////   bool sendPointCloud();
     //   bool send3D();
     //   void setSendKinectFrame(bool value);
    //    void setSendPointCloud(bool value);
     //   void setSend3D(bool value);
	void setSerial(QString value);
    /*! Create OpenNI work flow for camera number id and preallocaed data
      \param id device identificator
      */
    void setDeviceid(int id);
    void setflip(bool flip);
protected:
	void disconnect();
	virtual void receiveProcess(esp data);
    virtual bool getFrame(cv::Mat &mat);
private:
    static QList<int> _kinectlist;
    void checkXnError(openni::Status& status, const char* what) const;

    openni::VideoStream _depth,_color;

	bool m_high_resolution;
    bool m_mirrored;
	bool _connected;
    bool _flip;

    openni::Device _device;
	QString _serial;



	static QMutex _mutex;
    openni::VideoFrameRef _depthFrame;
    openni::VideoFrameRef _colorFrame;

};

#endif // KINECTOPENNI_H
