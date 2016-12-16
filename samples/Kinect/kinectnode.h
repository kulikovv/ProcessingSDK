#ifndef __KINECT_NODE__H__
#define __KINECT_NODE__H__

#include <QtCore/QMutex>

#include <io/capture/stereocameranode.h>

class KinectNode: public io::capture::StereoCameraNode
{
    Q_OBJECT
    Q_PROPERTY(int ID READ deviceID WRITE setDeviceID)
    Q_PROPERTY(QString Serial READ serial WRITE setSerial)
    Q_PROPERTY(double Tilt READ tilt WRITE setTilt)
    Q_PROPERTY(double AccelX READ accelX WRITE setAccelX)
    Q_PROPERTY(double AccelY READ accelY WRITE setAccelY)
    Q_PROPERTY(double AccelZ READ accelZ WRITE setAccelZ)
    Q_PROPERTY(bool SendIR2RGB READ sendIR2RGB WRITE setSendIR2RGB)
    Q_CLASSINFO("Send", "KinectFrame")

public:
    explicit KinectNode(QObject *parent = 0);
    ~KinectNode();
    int deviceID() const;
    const QString &serial() const;
    double accelX() const;
    double accelY() const;
    double accelZ() const;
    double tilt() const;
    bool sendIR2RGB() const;
    virtual esp get(const base::timestamp);

public slots:
    void setTilt(double);
    void setDeviceID(int);
    void setSerial(const QString &);
    void setAccelX(double);
    void setAccelY(double);
    void setAccelZ(double);
    void setSendIR2RGB(bool);

protected:
    virtual void receiveProcess(esp);
    virtual void successCallibration(camera::ICalibration *);
    virtual bool getFrame(cv::Mat &, cv::Mat &);

private:
    int m_id;
    base::timestamp m_posted;
    cv::Mat m_color;
    cv::Mat m_depth;
    mutable cv::Point3d m_pos;
    QString m_serial;
    bool m_autoStop;
    bool m_sendIR2RGB;
    QMutex m_mutex;

};

#endif // __KINECT_NODE__H__
