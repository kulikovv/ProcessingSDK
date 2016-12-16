#ifndef RGBDKINECT_H
#define RGBDKINECT_H

#include <QString>
#include <types/rgbd.h>
#include "kinectlibrary_global.h"

class KINECTLIBRARYSHARED_EXPORT RgbdKinect : public types::Rgbd
{
public:
    RgbdKinect(double pixel_size,double F);
    RgbdKinect(double pixel_size,double F,const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel);
    RgbdKinect(double pixel_size,double F,const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel, double angle);
    virtual void getPointCloud(cv::Mat &cloud);
    virtual cv::Point3f getPointAt(cv::Point2i pt);
    virtual cv::Point3f getPointAt(cv::Mat xx,cv::Point2i pt);
    virtual std::map<std::string,std::string> getConfig();
    virtual Rgbd *clone();
private:
    static float* _pointx;
    static float* _pointy;
    static float* buildArrayX();
    static float* buildArrayY();

    double _pixel_size;//! Размер пикселя
    double _F;//! Фокальное растояние

};

#endif // RGBDKINECT_H
