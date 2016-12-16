#include <libfreenect.h>
#include <libfreenect_sync.h>

#ifdef UNIX_BUILD
#define FREENECT_FRAME_W 640
#define FREENECT_FRAME_H 480
#define FREENECT_IR_FRAME_W 640
#define FREENECT_IR_FRAME_H 488
#define FREENECT_VIDEO_IR_8BIT_SIZE FREENECT_IR_FRAME_W*FREENECT_IR_FRAME_H
#define FREENECT_VIDEO_RGB_SIZE FREENECT_FRAME_W*FREENECT_FRAME_H*3
#define FREENECT_DEPTH_11BIT_SIZE sizeof(uint16_t)*FREENECT_FRAME_W*FREENECT_FRAME_H
#endif

#include <QDebug>

#include "tools.h"
#include "Kinect/kinectframe.h"
#include "Kinect/kinectsync.h"
#include "CV/pmat.h"
#include "../KinectSerial.hpp"
#include "kinectnode.h"

KinectNode::KinectNode(QObject *parent)
    : StereoCameraNode(parent),
      m_posted(0)
 {
    m_sendIR2RGB = true;
    setSendIR2RGB(false);
    setDeviceID(0);
    io::SourceNode::_fps = 25;
    io::SourceNode::_length = 0;
    m_depth.create(cv::Size(FREENECT_FRAME_W, FREENECT_FRAME_H), CV_16UC1);
}

KinectNode::~KinectNode()
{
    freenect_sync_stop();
}

int KinectNode::deviceID() const
{
    return m_id;
}

const QString &KinectNode::serial() const
{
    return m_serial;
}

double KinectNode::accelX() const
{
    return m_pos.x;
}

double KinectNode::accelY() const
{
    return m_pos.y;
}

double KinectNode::accelZ() const
{
    return m_pos.z;
}

double KinectNode::tilt() const
{
    freenect_raw_tilt_state *state;
    freenect_sync_get_tilt_state(&state, m_id);
    return freenect_get_tilt_degs(state);
}

bool KinectNode::sendIR2RGB() const
{
    return m_sendIR2RGB;
}

esp KinectNode::get(const io::timestamp)
{
    if (!m_mutex.tryLock())
        return esp();
    uint32_t ts;
    freenect_raw_tilt_state *state;
    freenect_sync_get_tilt_state(&state, m_id);
    freenect_get_mks_accel(state, &m_pos.x, &m_pos.y, &m_pos.z);
    int res1 = freenect_sync_get_depth((void**)&m_depth.data, &ts, m_id, FREENECT_DEPTH_11BIT);
    int res2 = freenect_sync_get_video((void**)&m_color.data, &ts, m_id, m_sendIR2RGB ? FREENECT_VIDEO_IR_8BIT : FREENECT_VIDEO_RGB);
    if(m_sendIR2RGB)
    {
        cv::blur(m_color,m_color,Size(5,5));
    }
    if (m_color.data && m_depth.data && res1 == 0 && res2 == 0) {
        emit send(esp::create<kinect::KinectFrame>(new kinect::KinectFrame(m_serial,
                                                                           m_color.clone(),
                                                                           m_depth.clone(),
                                                                           m_pos,
                                                                           freenect_get_tilt_degs(state)),
                                                   Types::releaseData<kinect::KinectFrame>,
                                                   m_posted++));
    }
    m_mutex.unlock();
    return esp();
}

void KinectNode::setTilt(double value)
{
    freenect_sync_set_tilt_degs(value, m_id);
}

void KinectNode::setDeviceID(int id)
{
    m_id = id;
    m_serial = getKinectSerial(m_id);
}

void KinectNode::setSerial(const QString &)
{
}

void KinectNode::setAccelX(double)
{
}

void KinectNode::setAccelY(double)
{
}

void KinectNode::setAccelZ(double)
{
}

void KinectNode::setSendIR2RGB(bool f)
{
    if (m_sendIR2RGB != f) {
        m_mutex.lock();
        m_sendIR2RGB = f;
        m_color.create(cv::Size(m_sendIR2RGB ? FREENECT_IR_FRAME_W : FREENECT_FRAME_W,
                                m_sendIR2RGB ? FREENECT_IR_FRAME_H : FREENECT_FRAME_H),
                       m_sendIR2RGB ? CV_8UC1 : CV_8UC3);
        m_mutex.unlock();
    }
}

void KinectNode::receiveProcess(esp data)
{
    //TODO: reaction on reciving command
    get(data.getFrameId());
}

void KinectNode::successCallibration(ICalibration *calib)
{
    camera::SterioCalibration *s = dynamic_cast<camera::SterioCalibration*>(calib);
    if (s)
        s->saveSettings(QString("%1.xml").arg(serial()).toStdString());
    emit io::CameraNode::sendFrame(cv::Mat());
}

bool KinectNode::getFrame(cv::Mat &left, cv::Mat &right)
{
    char *data = 0;
    uint32_t ts;
    if (freenect_sync_get_video((void**)&data, &ts, m_id, FREENECT_VIDEO_IR_8BIT) != 0)
        return false;
    left.create(cv::Size(FREENECT_IR_FRAME_W, FREENECT_IR_FRAME_H), CV_8UC1);
    memcpy(left.data, data, FREENECT_VIDEO_IR_8BIT_SIZE);
    if (freenect_sync_get_video((void**)&data, &ts, m_id, FREENECT_VIDEO_RGB) != 0)
        return false;
    right.create(cv::Size(FREENECT_FRAME_W, FREENECT_FRAME_H), CV_8UC3);
    memcpy(right.data, data, FREENECT_VIDEO_RGB_SIZE);
    return true;
}
