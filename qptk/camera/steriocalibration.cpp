#include "steriocalibration.h"
#include <QDebug>

#define SQUERE 0.0254f //Размер клетки в метрах
#define SX 8 //Кол-во клеток в сетке по X
#define SY 6 //Кол-во клеток в сетке по Y

#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace camera;
using namespace cv;

SterioCalibration::SterioCalibration()
{
    squareSize = SQUERE;
    //res = 0;
    pattern_size= Size(SX,SY);
    for(int i=0;i<pattern_size.height;i++)
    {
       for(int j=0;j<pattern_size.width;j++)
       {
            cv::Point3f pt = cv::Point3f(i*squareSize,j*squareSize,0);
            model_corners.push_back(pt);
       }
    }
    _left.init(model_corners,pattern_size);
    _right.init(model_corners,pattern_size);

}

SterioCalibration::SterioCalibration(const SterioCalibration &another)
    : _left(another._left),
    _right(another._right),
    model_corners(another.model_corners),
    pattern_size(another.pattern_size),
    imsize(another.imsize),
    R(another.R),
    T(another.T),
    E(another.E),
    F(another.F),
    squareSize(another.squareSize)
{
}

SterioCalibration::SterioCalibration(const cv::Mat &leftCam,
                                     const cv::Mat &leftDist,
                                     const cv::Mat &rightCam,
                                     const cv::Mat &rightDist,
                                     const cv::Mat &matR,
                                     const cv::Mat &matT)
{
    squareSize = SQUERE;
    //res = 0;
    pattern_size = Size(SX, SY);
    for (int i = 0; i < pattern_size.height; i++) {
       for (int j = 0; j < pattern_size.width; j++) {
            cv::Point3f pt = cv::Point3f(i * squareSize, j * squareSize, 0);
            model_corners.push_back(pt);
       }
    }
    _left.init(model_corners, pattern_size);
    _right.init(model_corners, pattern_size);
    _left.setCamMatrix(leftCam);
    _left.setDistCoef(leftDist);
    _right.setCamMatrix(rightCam);
    _right.setDistCoef(rightDist);
    R = matR;
    T = matT;
}

Calibration SterioCalibration::getCalib(cameras cam) const
{
    switch(cam)
    {
        case LEFT: return _left;
        case RIGHT: return _right;
    }
    return _left;
}

bool SterioCalibration::addImage(const cv::Mat left,const cv::Mat right)
{
    Mat x,y;
    return addImage(left,right,x,y);
}

bool SterioCalibration::addImage(const cv::Mat left,const cv::Mat right,cv::Mat &resultLeft,cv::Mat &resultRight)
{
    imsize = Size(left.cols,left.rows);
    bool lsuc = _left.addImage(left,resultLeft);
    bool rsuc = _right.addImage(right,resultRight);
    if(true==lsuc&&true==rsuc)
    {
        return true;
    }else{
        if(true==lsuc)
        {
            //qDebug()<<"Left remove";
            _left.removeLast();
        }
        if(true==rsuc)
        {
            //qDebug()<<"Right remove";
            _right.removeLast();
        }
    }
    return false;
}

/*void SterioCalibration::setCallibrationWidget(CallibrationResult* cw)
{
        res = cw;
}*/

void SterioCalibration::saveSettings(std::string file) const
{
    cv::FileStorage fs(file,cv::FileStorage::WRITE);
    fs << "LeftIntistics" << _left.getCamMatrix();;
    fs << "LeftDist" << _left.getDistCoef();
    fs << "RightIntistics" << _right.getCamMatrix();
    fs << "RightDist" << _right.getDistCoef();
    fs << "R" << R;
    fs << "T" << T;
}
void SterioCalibration::removeLast()
{
     _left.removeLast();
     _right.removeLast();
}
void SterioCalibration::clear()
{
    _left.clear();
    _right.clear();
    R.setTo(0);
}

void SterioCalibration::loadSettings(std::string file)
{
    cv::FileStorage fs(file,cv::FileStorage::READ);
    Mat in,dist,in2,dist2;
    fs["LeftIntistics"] >> in;
    fs ["LeftDist"]>> dist;
    _left.setCamMatrix(in);
    _left.setDistCoef(dist);
    fs ["RightIntistics"]>> in2;
    fs ["RightDist"] >>dist2;
    _right.setCamMatrix(in2);
    _right.setDistCoef(dist2);
    fs ["R"] >> R;
    fs ["T"] >> T;
}

bool SterioCalibration::callibrate()
{
    bool lsuc = _left.callibrate();
    bool rsuc = _right.callibrate();
    if(true==lsuc&&true==rsuc)
    {      
        //qDebug() << "Right & Left calibrate";
        std::vector< std::vector<Point3f> > model;
        for(unsigned int i=0;i<_left.getCorners().size();i++)
        {
            model.push_back(model_corners);
        }
        Mat lcm = _left.getCamMatrix();
        Mat rcm = _right.getCamMatrix();
        Mat ldk = _left.getDistCoef();
        Mat rdk = _right.getDistCoef();        
        cv::stereoCalibrate(model,_left.getCorners(),_right.getCorners(),lcm,
                            ldk,rcm,rdk,imsize,R,T,E,F,
                            CALIB_ZERO_TANGENT_DIST +
                            CALIB_FIX_INTRINSIC+
                            CALIB_FIX_K3,
                            TermCriteria(TermCriteria::MAX_ITER+TermCriteria::EPS, 100, 1e-8));

        return true;
    }
    return false;
}
Mat SterioCalibration::getT() const
{
    return T;
}

Mat SterioCalibration::getR() const
{
    return R;
}
