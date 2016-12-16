#include "rgbdkinect.h"
#include <utils/logs.h>
#define ROWS 480
#define COLS 640


float* RgbdKinect::_pointx = buildArrayX();
float* RgbdKinect::_pointy = buildArrayY();

float* RgbdKinect::buildArrayX()
{
    float* ptr = new float[COLS*ROWS];
    for(int i=0;i<COLS*ROWS;i++)
    {
        int x=i%COLS;
        ptr[i] = x - COLS/2;
    }


    return ptr;
}
types::Rgbd *RgbdKinect::clone()
{
    return new RgbdKinect(_pixel_size,_F,m_serial,m_rgb.clone(),m_depth.clone(),m_pos);
}

float* RgbdKinect::buildArrayY()
{
    float* ptr = new float[COLS*ROWS];
    for(int i=0;i<COLS*ROWS;i++)
    {
        int y=i/COLS;
        ptr[i] = y - ROWS/2;
    }

    return ptr;
}

RgbdKinect::RgbdKinect(double pixel_size, double F):types::Rgbd()
{
    _pixel_size = pixel_size;
    _F = F;
}
RgbdKinect::RgbdKinect(double pixel_size,double F,const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel):
    types::Rgbd(serial,rgb,depth,accel)
{
    _pixel_size = pixel_size;
    _F = F;
}

RgbdKinect::RgbdKinect(double pixel_size,double F,const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel, double angle):
    types::Rgbd(serial,rgb,depth,accel,angle)
{
    _pixel_size = pixel_size;
    _F = F;
}


cv::Point3f RgbdKinect::getPointAt(cv::Point2i pt)
{
    cv::Point3f pt3d;
    double tempm = _pixel_size * 0.001f / _F;

    unsigned short* depth = m_depth.ptr<unsigned short>(0);

    int i=pt.y*m_depth.cols+pt.x;

    pt3d.x = _pointx[i] * depth[i]* tempm;
    pt3d.y = _pointy[i] * depth[i]* tempm;
    pt3d.z = depth[i] * 0.001f;

    return pt3d;
}

cv::Point3f RgbdKinect::getPointAt(cv::Mat xx, cv::Point2i pt)
{
    cv::Point3f pt3d;
    double tempm = _pixel_size * 0.001f / _F;

    unsigned short* depth = m_depth.ptr<unsigned short>(0);

    int i=pt.y*m_depth.cols+pt.x;

    pt3d.x = _pointx[i] * depth[i]* tempm;
    pt3d.y = _pointy[i] * depth[i]* tempm;
    pt3d.z = depth[i] * 0.001f;

    return pt3d;
}

void RgbdKinect::getPointCloud(cv::Mat &cloud)
{
    if(cloud.empty())
    {
        cloud.create(ROWS,COLS,CV_32FC3);
    }

    //g_depth.GetIntProperty ("ZPD", F_);
    // get the pixel size in mm ("ZPPS" = pixel size at zero plane)
   // g_depth.GetRealProperty ("ZPPS", pixel_size_);

  //  qDebug() << "RgbdKinect::getPointCloud";
    double tempm = _pixel_size * 0.001f / _F;
    float* ptr = cloud.ptr<float>(0);//cloud pointer
    unsigned short* depth = m_depth.ptr<unsigned short>(0);//depth pointer
    for (int i=0;i<COLS*ROWS;i++)
    {
        ptr[i*3] = depth[i] * 0.001f; // from mm in meters!
        ptr[i*3+1] = _pointx[i] * depth[i]* tempm;
        ptr[i*3+2] = _pointy[i] * depth[i]* tempm;
    }
}

std::map<std::string,std::string> RgbdKinect::getConfig()
{
    std::map<std::string,std::string> map = Rgbd::getConfig();
    map["ZPD"]=QString("%1").arg(_F).toStdString();
    map["ZPPS"]=QString("%1").arg(_pixel_size).toStdString();
    return map;
}
