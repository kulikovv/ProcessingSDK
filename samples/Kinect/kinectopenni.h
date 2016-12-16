#ifndef KINECTOPENNI_H
#define KINECTOPENNI_H

#include <XnOS.h>
#include <math.h>

#include <XnCodecIDs.h>
#include <XnCppWrapper.h>
using namespace xn;
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
	Q_CLASSINFO("Send", "KinectFrame")
public:
	explicit KinectOpenNI(QObject *parent);
	~KinectOpenNI();

    virtual esp get(const base::timestamp frame);
    virtual bool isConnected();
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
protected:
	void disconnect();
	virtual void receiveProcess(esp data);
    virtual bool getFrame(cv::Mat &mat);
private:
    static QList<int> _kinectlist;
	void checkXnError(const XnStatus& status, const char* what) const;
    /*! Function calculate realworld 3D coordinates from given depth map
      \param depth depth map recived form microsoft kinect
      \return pointer to realworld data that have been PREALLOCATED in setID() function do NOT DELETE IT (this pointer can be retrived by accesing global parameter _real)
      */
    XnPoint3D *retrievePointCloudMap(const XnDepthPixel *depth);
    /*! Convert
      \param real pointer to realworld coordinates in
      \param size

      */
    cv::vector<cv::Point3f>* xnPoint3DToPointCloud(XnPoint3D *real,int size);
    cv::Mat retrievePointCloudAsImage(const XnDepthPixel *depth);
	Context g_context;
	DepthGenerator g_depth;
	ImageGenerator g_image;
	DepthMetaData g_depthMD;
	ImageMetaData g_imageMD;

	bool m_high_resolution;
    bool m_mirrored;
	bool _connected;

	Device g_device;
	QString _serial;

    bool _sendKinectFrame;
    bool _sendPointCloud;
    bool _send3d;

	static QMutex _mutex;

   // std::vector<cv::Point3f> *_cloud;
    XnPoint3D* _proj;//!< Preallocated array to store kinect camera coordinates
    XnPoint3D* _real;//!< Preallocaed array for storing realworld coordinates
};

#endif // KINECTOPENNI_H
