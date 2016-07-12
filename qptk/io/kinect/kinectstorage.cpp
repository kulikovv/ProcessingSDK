/*
 * Kinect Player
 * Kulikov Victor
 * kulikov.victor@gmail.com
 * GNU Lisence 2011
 */

#define FREENECT_IR_FRAME_W 640
#define FREENECT_IR_FRAME_H 480
#define CUR_VERSION 2
#include "kinectstorage.h"
#include <vector>
#include <stdio.h>
#include <cstring>
#include <QDebug>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <types/rgbd.h>
#include <base/esp.h>


using namespace io::kinect;
using namespace types;
using namespace cv;
using namespace base;

KinectStorage::KinectStorage(std::string fname,mode mod,int fps)
{

    this->fps = fps;
    lenght=0;
    KinectStorage::header head;
    head.version=CUR_VERSION;
    head.fps=fps;
    head.lenght=lenght;
    head.offset=0;
    curpos=0;

    outfile=0;
    infile =0;

    /* // Maybe resampling of depth data is needed
    for (int i=0; i<2048; i++) {
           float v = i/2048.0;
           v = powf(v, 3)* 6;
           t_gamma[i] = v*6*256;
    }*/
    positions.clear();


    switch(mod)
    {
        case KinectStorage::CLOSED:
            break;
        case KinectStorage::WRITE:
            curmod = WRITE;           
            outfile =new std::ofstream(fname.c_str(), std::ios::out | std::ios::binary);
            if(!outfile)
            {
                throw 2;
            }
            outfile->write((const char *)&head,sizeof(KinectStorage::header));
            break;
        case KinectStorage::READ:
            //Reading header
            curmod = READ;
            infile =new std::ifstream(fname.c_str(), std::ios::in | std::ios::binary);
            if(!infile->is_open())
            {
                throw 3;
            }
            infile->read((char *)&head,sizeof(KinectStorage::header));
            _ver = head.version;
            if(head.version>CUR_VERSION)
            {
                throw 1;
            }
            fps = head.fps;
            lenght = head.lenght;
            //Reading positions        
            infile->seekg(head.offset);
            positions = readVector<uint32_t>();
            //infile->read((char *)&positions.front(), sizeof(uint32_t)*dsz);
            //Seek back to the begin
            infile->seekg(sizeof(KinectStorage::header));
            break;

    }
}
bool KinectStorage::split16BitFrame(Mat &src,Mat &dst1,Mat &dst2)
{
    MatIterator_<ushort> it = src.begin<ushort>();
    MatIterator_<ushort> it_end = src.end<ushort>();
    MatIterator_<uchar> it2 = dst1.begin<uchar>();
    MatIterator_<uchar> it3 = dst2.begin<uchar>();

    for(; it != it_end; ++it)
    {
        int pval = *it;
        int lb = pval & 0xff;
        *it2 = saturate_cast<uchar>(pval>>8);
        *it3 = saturate_cast<uchar>(lb);
        ++it2;
        ++it3;
    }

    return true;
}
int KinectStorage::getFps()
{
    return this->fps;
}

bool KinectStorage::merge16BitFrame(Mat &src1,Mat &src2,Mat &dest)
{
    if(0==dest.data)
    {
        dest.create(cv::Size(src1.cols,src1.rows),CV_16UC1);
    }
    MatIterator_<uchar> it = src1.begin<uchar>();
    MatIterator_<uchar> it_end = src1.end<uchar>();
    MatIterator_<uchar> it2 = src2.begin<uchar>();

    MatIterator_<ushort> it3 = dest.begin<ushort>();

    for(; it != it_end; ++it)
    {


        int z = *it<<8;
        int lb = *it2;
        *it3 = saturate_cast<ushort>(z+lb);

        ++it2;
        ++it3;

    }

    return true;
}
template <class T> inline void  KinectStorage::writeVector(std::vector<T> data)
{
    std::size_t dsz = data.size();
    outfile->write((const char *)&dsz, sizeof(size_t));
    outfile->write((const char *)&data.front(), data.size()*sizeof(T));
}
template <class T>  inline std::vector<T> KinectStorage::readVector()
{

    size_t dsz=0;
    std::vector<T> data;
    data.clear();
    infile->read((char *)&dsz, sizeof(size_t));
    data.resize(dsz);
    infile->read((char *)&data.front(), sizeof(T)*dsz);
    return data;
}

inline void KinectStorage::writeImage(std::string ext,const Mat& img)
{
    static std::vector<uchar> data;
    if(cv::imencode(ext,img,data))
    {
        writeVector(data);
    }
}
inline Mat KinectStorage::readImage(int flag)
{
    std::vector<uchar> data = readVector<uchar>();
    Mat dp(data);
    return  cv::imdecode(dp,flag);
}
bool KinectStorage::write(types::Rgbd *frame)
{
    if(!frame)
    {
        return false;
    }
    char serial[SERIAL_LENGTH];
    memset(&serial,0,sizeof(char)*SERIAL_LENGTH);
    memcpy(serial,frame->serial().toStdString().c_str(),sizeof(char)*SERIAL_LENGTH);
    return write(0,frame->rgb(),frame->depth(),serial,point3d2vector(frame->accel()));
}

bool KinectStorage::write(uint32_t ts,Mat rgb,Mat depth,char* serial,std::vector<double> accel)
{
    if(!outfile||(curmod!=WRITE))
    {
        return false;
    }

    positions.push_back(outfile->tellp());

    static Mat dst1(Size(FREENECT_IR_FRAME_W,FREENECT_IR_FRAME_H),CV_8UC1);
    static Mat dst2(Size(FREENECT_IR_FRAME_W,FREENECT_IR_FRAME_H),CV_8UC1);

    outfile->write((const char *)&ts,sizeof(uint32_t));
    outfile->write(serial, sizeof(char)*SERIAL_LENGTH);

    writeVector<double>(accel);

    split16BitFrame(depth,dst1,dst2);
    writeImage(".png",dst1);
    writeImage(".png",dst2);
    writeImage(".jpg",rgb);
    lenght++;
    return true;
}
bool KinectStorage::read(Mat &rgb,Mat &depth,long pos)
{
    std::vector<double> trash;//throw away for usage with old interface
    char serial[SERIAL_LENGTH];
    return read(rgb,depth,serial,trash,pos);
}
std::vector<double> KinectStorage::point3d2vector(cv::Point3d point)
{
    std::vector<double> accel(3);

    accel.push_back(point.x);
    accel.push_back(point.y);
    accel.push_back(point.z);
    return accel;
}

cv::Point3d KinectStorage::vector2point3d(std::vector<double> vector)
{
    if(3!=vector.size())
    {
        return cv::Point3d(0,0,0);
    }
    return cv::Point3d(vector[0],vector[1],vector[2]);

}
types::Rgbd* KinectStorage::read(long pos)
{
    Mat rgb,depth;
    std::vector<double> tmp;
    char serial[SERIAL_LENGTH];
    if(!read(rgb,depth,serial,tmp,pos))
        return false;
    /*if(frame)
    {
        delete frame;
        frame = 0;
    }*/
    QString str = "";
    if (_ver>1)
    {
        str = QString::fromStdString(std::string(serial));
    }

    return new types::Rgbd(str,rgb,depth,vector2point3d(tmp));
}

bool KinectStorage::read(Mat &rgb,Mat &depth,char* serial,std::vector<double> &accel,long pos)
{
    if(!infile||(curmod!=READ))
    {
        return false;
    }


    uint32_t ts;


    if(pos>=0)
    {
        curpos=pos;
        infile->seekg(positions[curpos]);
    }
    if(curpos>=length())
    {
        return false;
    }

    infile->read((char *)&ts, sizeof(uint32_t));
    if (_ver>1)
    {
        infile->read(serial, sizeof(char)*SERIAL_LENGTH);
    }else{
        memset(serial,0,sizeof(char)*SERIAL_LENGTH);
    }

    accel = readVector<double>();


    static Mat src1(Size(FREENECT_IR_FRAME_W,FREENECT_IR_FRAME_H),CV_8UC1);
    static Mat src2(Size(FREENECT_IR_FRAME_W,FREENECT_IR_FRAME_H),CV_8UC1);

    src1 = readImage();
    src2 = readImage();
    rgb = readImage(1);
    this->merge16BitFrame(src1,src2,depth);
    curpos++;
    return true;
}

uint32_t KinectStorage::length()
{
    return lenght;
}

KinectStorage::~KinectStorage()
{
    close();
}
void KinectStorage::close()
{
    curmod = CLOSED;
    if(outfile)
    {
        KinectStorage::header head;
        head.version=CUR_VERSION;
        head.offset = outfile->tellp();
        //Saving frames positions
        writeVector<uint32_t>(positions);
        //Rewrite header
        outfile->seekp(0);
        head.fps=fps;
        head.lenght=lenght;
        outfile->write((const char *)&head,sizeof(KinectStorage::header));
        //Close the stream
        delete outfile;
        outfile=NULL;
    }
    if(infile)
    {
        delete infile;
        infile=NULL;
    }
}
