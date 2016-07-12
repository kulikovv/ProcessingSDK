// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.

#ifndef PROCESSINGFRAMEWORK_KINECTSTORAGE_H
#define PROCESSINGFRAMEWORK_KINECTSTORAGE_H
#include <string>
#include <vector>
#include <fstream>
#include <opencv2/core/core.hpp>
#include <sdk_global.h>

#ifndef WIN32_MSVC
#include <stdint.h>
#else
#include <MSVC/stdint.h>
#endif

#define SERIAL_LENGTH 18

namespace types
{
    class Rgbd;
}
namespace io
{
namespace kinect
{
/*! Class KinectStorage store compressed data from kinnetsensor
 * into file Depth inforamtion if compressed lossless using PNG
 *algorithm and RGB data is compressed using JPEG algorithm.
 * Compessed data weight is about 5Mb/sec
 */
class PROCESSINGFRAMEWORK_SHARED_EXPORT KinectStorage
{
    /*! File header:
      * version - file forcv::Mat version
      * lenght - number of frames
      * fps - frames par second
      * offset - start of annotation vector
      */
    struct header{
        uint32_t version;
        uint32_t lenght;
        int fps;
        uint32_t offset;
    };

public:
    /*! File open mode
      */
    enum mode{
        CLOSED,//!< File is closed
        READ,//!< Reading of file
        WRITE //!< Writing file
    };
    /*! Creation of kinectStorage
      \param fname - the name of the file
      \param mod - reading/writing mode
      \param fps = frames par second
      */
    KinectStorage(std::string fname,mode mod=READ,int fps=25);
    /*! Write in file compressed into stream
      * \param ts - timestamp
      * \param rgb - color image
      * \param depth - the depth image from kinect sensor
      * \return true is succeeded. false otherwise
      */
    bool write(uint32_t ts,cv::Mat rgb,cv::Mat depth,char *serial,std::vector<double> accel=std::vector<double>());
    /*! Redefinition using types::Rgbd class
      \sa write
      \sa types::Rgbd
      */
    bool write(types::Rgbd* frame);
    /*! Read a frame from opened storage
    * \param rgb - reference to color image (May be not initiated)
    * \param depth - reference to depth image from kinect sensor (May be not initiated)
    * \param pos - position of the image infile for 0 to length-1
    * not implemented yet. -1 means to take next frame.
    * \return true is succeeded. false otherwise
    */
    bool read(cv::Mat &rgb,cv::Mat &depth,long pos=-1);
    /*! Added accel data
      * \param accel - accelerometr vector
      * \sa read
      */
    bool read(cv::Mat &rgb,cv::Mat &depth,char* serial,std::vector<double> &accel,long pos=-1);
    /*! Read image as types::Rgbd
      * \sa read
      * \sa types::Rgbd
      */
    types::Rgbd* read(long pos=-1);
    /*! Return file length
      */
    uint32_t length();
    /*! Deallocate memory save headers into file
      */
    ~KinectStorage();
    /*! Return frames par second
      */
    int getFps();
    /*! Close the file
      */
    void close();

    /*! Converts cv::Point3d to std::vector
      \param point - a point in 3d space
      \return vector with 3 elements and double type
      */
    std::vector<double> point3d2vector(cv::Point3d point);
    /*! Converts std::vector to cv::Point3d
      \param vector - vector with 3 elements
      \return point in 3d space
      */
    cv::Point3d vector2point3d(std::vector<double> vector);

private:
    /*! Slipts 16bit image (depth image) into two 8bit image
      upper byte and lower byte
      \param src - 16bit source image
      \param dst1 - 8bit image upper byte
      \param dst2 - 8bit image lower byte
      \return true is succeeded. false otherwise
      */
    bool split16BitFrame(cv::Mat &src,cv::Mat &dst1,cv::Mat &dst2);
    /*! Merge into 16bit image (depth image) from two 8bit image
      upper byte and lower byte
      \param src1 - 8bit image upper byte
      \param src2 - 8bit image lower byte
      \param dest - 16bit image
      \return true is succeeded. false otherwise
      */
    bool merge16BitFrame(cv::Mat &src1,cv::Mat &src2,cv::Mat &dest);
    /*! Write image into file
      *\param ext - file extension (for example ".png")
      *\param img - the image to save (8 bit par chanel)
      */
    inline void writeImage(std::string ext,const cv::Mat& img);
    /*! Read the current image
      *\return allocated decompressed image
      */
    inline cv::Mat readImage(int flag=0);
    /*! Write the current vector
      *\return allocated decompressed image
      */
    template <class T> inline void writeVector(std::vector<T> data);
    /*! Read the current vector
      *\return allocated decompressed image
      */
    template <class T> inline std::vector<T> readVector();

    uint32_t lenght; //!< The length of the file
    uint32_t curpos; //!< Current position
    int fps; //!< Frames par second
    mode curmod; //!< Current saving mode
    std::ofstream *outfile; //!< File output stream
    std::ifstream *infile; //!< File input stream
    std::vector<uint32_t> positions; //!< Frames positions in file

    uint16_t t_gamma[2048];//!< Gamma correction (not used now)
    int _ver;//!< Version of the readed file
};
}
}
#endif // KINECTSTORAGE_H
