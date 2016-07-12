#include "stereocameranode.h"
#include <camera/steriocalibration.h>
using namespace io;
using namespace io::capture;
using namespace camera;

StereoCameraNode::StereoCameraNode(QObject *parent) :
    CameraNode(parent)
{
   // _calib = new SterioCalibration();
}

cv::Mat StereoCameraNode::joinPics(cv::Mat left,cv::Mat right)
{
    int rows = left.rows>right.rows?left.rows:right.rows;
    int cols = left.cols + right.cols;
    cv::Mat x(rows,cols,CV_8UC3);
    x.setTo(0);
    cv::Mat roi(x, cv::Rect(0,0,left.cols,left.rows));
    left.copyTo(roi);
   //roi = left;
    cv::Mat roi2(x, cv::Rect(left.cols,0,right.cols,right.rows));
    right.copyTo(roi2);
   // roi2 = right;
    return x;
}

void StereoCameraNode::frameRequest()
{
    //SterioCalibration* sc = (SterioCalibration*)_calib;
    cv::Mat left,right,lres,rres;
    if(getFrame(left,right))
    {
        lres=left.clone();
        rres=right.clone();
    //    sc->addImage(left,right,lres,rres);
        //emit sendFrame(joinPics(lres,rres));
    }
}


