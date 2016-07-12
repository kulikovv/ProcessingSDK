// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.
#ifndef PROCESSINGFRAMEWORK_KINECT_FRAME__H__
#define PROCESSINGFRAMEWORK_KINECT_FRAME__H__

#include <QtCore/QString>
#include <sdk_global.h>
#include <opencv2/core/core.hpp>
#include <types/rgb.h>
#include <map>

namespace types
{
/*! Represent a kinect frame.
  * On initialization it grabs frame
 */
class PROCESSINGFRAMEWORK_SHARED_EXPORT Rgbd
{
public:
    /*! Create empty kinectframe
      */
    Rgbd();

    /*! Create Kinect frame from memory storage
      * \param serial - kinect serial number
      * \param rgb - rgb frame
      * \param depth - depth frame
      * \param accel - accelerometr params
      */
    Rgbd(const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel);
    Rgbd(const QString &serial, const cv::Mat &rgb, const cv::Mat &depth, const cv::Point3f &accel, double angle);

    /*!
      Redefinition of copy operator
      */
    Rgbd(const Rgbd &another);

    /*!
      Redefinition of equal operator
      */
    Rgbd &operator=(const Rgbd &another);
    /*! Recive color image
     *  \return rgb frame (3x8bit)
     */
    Rgb rgb() const;
    /*! Recive depth image
       * \return depth frame in pixels (16bit)
      */
    Gray depth() const;
    operator Rgb(){ return rgb(); }
    cv::Point3f accel() const; //!< Get accelerometer value
    double angle() const; //!< Tilt angle
    const QString &serial() const; //!< Retrive serial
    bool isEmpty() const; //!< Return true if the frame is empty, false otherwise
    /*!
     * \brief getPointCloud return a 3 chanel image with coordinates in meters fo each pixel
     * \param cloud if the cloud is initialized it is filled with 3d points, otherwise in is allocated
     */
    virtual void getPointCloud(cv::Mat &cloud);
    virtual cv::Point3f getPointAt(cv::Point2i pt);
    virtual cv::Point3f getPointAt(cv::Mat cloud,cv::Point2i pt);
    virtual std::map<std::string,std::string> getConfig();
    /*!
     * \brief clone copy everything
     * \return
     */
    virtual Rgbd* clone();
protected:
    cv::Mat m_rgb; //!< Color image
    cv::Mat m_depth; //!< Depth image
    cv::Point3f m_pos; //!< Position
    cv::Point3f m_rotation;//!< Rotation
    double m_angle;
    QString m_serial; //!< Serial number of camera
};

} // namespace types
/*namespace kinect
{
    typedef types::Rgbd KinectFrame;
}*/
#endif // __KINECT_FRAME__H__
