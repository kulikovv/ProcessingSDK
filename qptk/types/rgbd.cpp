#include <types/rgbd.h>
#include <QDebug>
#ifndef WIN32_MSVC
# include <stdint.h>
#else
# include "MSVC/stdint.h"
#endif // WIN32_MSVC
#include "utils/logs.h"
using namespace types;

Rgbd::Rgbd()
    : m_pos(cv::Point3f(0.0, 0.0, 0.0)),
    m_angle(0.0)
{
}

Rgbd *Rgbd::clone()
{
    return new Rgbd(m_serial,m_rgb.clone(),m_depth.clone(),m_pos);
}

Rgbd::Rgbd(const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel)
    : m_rgb(rgb),
    m_depth(depth),
    m_pos(accel),
    m_angle(0.0),
    m_serial(serial)
{
}

Rgbd::Rgbd(const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel, double angle)
    : m_rgb(rgb),
    m_depth(depth),
    m_pos(accel),
    m_angle(angle),
    m_serial(serial)
{
}



Rgbd::Rgbd(const Rgbd &another)
    : m_rgb(another.m_rgb),
    m_depth(another.m_depth),
    m_pos(another.m_pos),
    m_angle(another.m_angle),
    m_serial(another.m_serial)
{
}

Rgbd &Rgbd::operator=(const Rgbd &another)
{
    m_rgb = another.m_rgb;
    m_depth = another.m_depth;
    m_pos = another.m_pos;
    m_angle = another.m_angle;
    m_serial = another.m_serial;
    return (*this);
}

Rgb Rgbd::rgb() const
{
    return Rgb(m_rgb);
}

Gray Rgbd::depth() const
{
    return Gray(m_depth);
}

cv::Point3f Rgbd::accel() const
{
    return m_pos;
}

double Rgbd::angle() const
{
    return m_angle;
}

const QString &Rgbd::serial() const
{
    return m_serial;
}

bool Rgbd::isEmpty() const
{
    return !m_rgb.data && !m_depth.data;
}

void Rgbd::getPointCloud(cv::Mat &cloud)
{
    Q_UNUSED(cloud)
    //INFO("Default behaviour");
    //by default -> do nothing
}

cv::Point3f Rgbd::getPointAt(cv::Point2i pt)
{
    Q_UNUSED(pt)
    return cv::Point3f(0,0,0);
}

cv::Point3f Rgbd::getPointAt(cv::Mat cloud,cv::Point2i pt)
{
    return cloud.at<cv::Point3f>(pt);
}

std::map<std::string,std::string> Rgbd::getConfig()
{
    std::map<std::string,std::string> m;
    m["SERIAL"]=m_serial.toStdString();
    m["POSITION"]=QString("%1 %2 %3").arg(m_pos.x).arg(m_pos.y).arg(m_pos.z).toStdString();
    m["ANGLE"]=QString("%1").arg(m_angle).toStdString();
    return m;
}
