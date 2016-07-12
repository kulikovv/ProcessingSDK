#include "calibration.h"
#include <QDebug>



#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <math.h>
using namespace camera;
using namespace cv;
using namespace std;

Calibration::Calibration()
{
    squareSize = 1.f;
    pattern_size= Size(8,6);
    cameraMatrix = Mat::eye(3, 3, CV_64F);
    _normalizeImage = true;
    _repError = -1;
    _reduceResolution = true;
    _ptype = 0;
    generateObject();
}
void Calibration::setPatternType(int pt)
{
    _ptype = pt;
}

int Calibration::getPatternType() const
{
    return _ptype;
}
double Calibration::getMeanReprojectionError() const
{
    return _repError;
}
void Calibration::removeLast()
{
    if(detected_corners.size()>0)
    {
       // detected_corners.erase(detected_corners.end()-1 );
        detected_corners.pop_back();
    }
}

void Calibration::init(std::vector<Point3f> model,Size pattern)
{
    pattern_size= pattern;
    model_corners = model;
}
bool Calibration::addImage(const cv::Mat im)
{
    Mat x;
    return addImage(im,x);
}

bool Calibration::addImage(const cv::Mat im,cv::Mat &result)
{
    qDebug() << "addImage";
    if(im.empty())
    {
        qDebug() << "Empty image exit";
        return false;
    }

    Mat grey;
    imsize = Size(im.rows,im.cols);
   // grey.create(imsize,CV_8UC1);

    if(1!=im.channels())
    {

        cvtColor(im, grey, cv::COLOR_BGR2GRAY);
      //  im.convertTo(grey,CV_8UC1);
        result = im.clone();
    }else{
        //Р Р°Р·РјР°Р·С‹РІР°РµРј, С‡С‚РѕР±С‹ РґРµС‚РµРєС‚РѕСЂ РЅРµ РѕС‚РІР»РµРєР°Р»СЃСЏ РЅР° РјРµР»РєРёРµ С‚РѕС‡РєРё
       // cv::blur(im,grey,Size(6,6));
        grey = im.clone();

   //     result.create(cv::Size(grey.cols,grey.rows),CV_8UC3);
   //     result.setTo(0);
        cv::cvtColor(grey,result,cv::COLOR_GRAY2BGR);
      /*  std::vector<cv::Mat> ls;
        ls.push_back(grey);
        ls.push_back(grey);
        ls.push_back(grey);
        cv::merge(ls,result);*/
    }

    imsize = Size(grey.cols,grey.rows);

    std::vector<Point2f> detected;
    double fx = 1;
    double fy = 1;
    try{

        qDebug() << "findChessboardCorners";
        cv::Mat small_gray;
        if(_reduceResolution&&grey.cols>640&&(0==_ptype))
        {
            fx = 640.0f/grey.cols;
            fy = 480.0f/grey.rows;
            cv::resize(grey,small_gray,cv::Size(640,480));
        }else{
            small_gray = grey;
        }

        bool found=false;
        //,CV_CALIB_CB_NORMALIZE_IMAGE+CALIB_CB_FAST_CHECK CV_CALIB_CB_ADAPTIVE_THRESH
        switch(_ptype)
        {
            case 0:
                found=cv::findChessboardCorners(small_gray,pattern_size,detected, CALIB_CB_ADAPTIVE_THRESH|(_normalizeImage&CALIB_CB_NORMALIZE_IMAGE));

            break;
            case 1:
                found=cv::findCirclesGrid(small_gray,pattern_size,detected,CALIB_CB_SYMMETRIC_GRID);
            break;
            case 2:
                found=cv::findCirclesGrid(small_gray,pattern_size,detected,CALIB_CB_ASYMMETRIC_GRID);
            break;
        }
        if(!found)
        {
            qDebug() << "Not found";
            return false;
        }


        for(unsigned int i=0;i<detected.size();i++)
        {
            cv::Point2f pt = detected[i];
            detected[i] = cv::Point2f(pt.x/fx,pt.y/fy);
        //    qDebug() << pt.x << pt.x/fx << pt.y << pt.y/fy;
        }
        if(0==_ptype)
        {
            if(detected.size()==(unsigned)(pattern_size.width*pattern_size.height))
            {
                qDebug() << "Start presize";
                TermCriteria termCriteria = TermCriteria(cv::TermCriteria::MAX_ITER+cv::TermCriteria::EPS, 1000, 0.0001);

                std::vector<Point2f> detected2(detected.begin(),detected.end());


                cv::cornerSubPix(grey,detected,Size(2,2),Size(-1,-1),termCriteria);
              //  cv::cornerSubPix(grey,detected,Size(2,2),Size(-1,-1),termCriteria);

                cv::cornerSubPix(grey,detected2,Size(5,5),Size(-1,-1),termCriteria);
                std::vector<Point2f> dis;

                //cv::absdiff(cv::Mat(detected),cv::Mat(detected2),cv::Mat(dis));
              //  cv::Scalar s = cv::sum(dis);
               // cv::Scalar m =cv::mean(dis);

              //  qDebug() << s[0] << s[1] << s[2]<< m[0] << m[1] << m[2];

            }
        }
        detected_corners.push_back(detected);
        cv::drawChessboardCorners(result,pattern_size,detected,true);
        return true;


    }catch(...)
    {
        qDebug() << "Exeption";
    }


    return false;
}
void Calibration::setNormalization(bool value)
{
    _normalizeImage = value;
}
bool Calibration::getReduceResolution() const
{
    return _reduceResolution;
}

void Calibration::setReduceResolution(bool value)
{
    _reduceResolution = value;
}

bool Calibration::callibrate()
{

    if(detected_corners.size()<1)
    {
        return false;
    }
   // cv::FileStorage fs("matr.xml",cv::FileStorage::WRITE);
    std::vector< std::vector<Point3f> > model;
    for(unsigned int i=0;i<detected_corners.size();i++)
    {
        model.push_back(model_corners);
    }

    std::vector<cv::Mat> rvect;
    std::vector<cv::Mat> tvect;

    qDebug() << "calib"<< detected_corners.size();

    cv::calibrateCamera(model,
                        detected_corners,
                        imsize,
                        cameraMatrix,
                        distCoef,
                        rvect,
                        tvect,
                        cv::CALIB_FIX_K3);
    std::vector<float> errPerView;
    _repError = computeReprojectionErrors(model,detected_corners,rvect,tvect,cameraMatrix,distCoef,errPerView);
    initUndistort();
    detected_corners.clear();
    return true;
}

std::vector<std::vector<Point2f> > Calibration::getCorners()
{
    return detected_corners;
}

Mat Calibration::getCamMatrix() const
{
    return cameraMatrix;
}

/*! */
Mat Calibration::getDistCoef() const
{
    return distCoef;
}

Mat Calibration::undistort(Mat x)
{
    Mat out;
    cv::remap(x,out,dx,dy,INTER_CUBIC);
    return out;
}
Mat Calibration::distort(Mat x)
{
    Mat out;
    cv::remap(x,out,idx,idy,INTER_CUBIC);
    return out;
}
void Calibration::clear()
{
    detected_corners.clear();
}

void Calibration::setCamMatrix(Mat cam)
{
    cameraMatrix = cam;
    initUndistort();
}

void Calibration::setDistCoef(Mat dist)
{
    distCoef = dist;
    initUndistort();
}

Point2f Calibration::undistManual(Point2f coord, Mat distcoef, bool /*useintistics*/)
{
  //  qDebug() << cameraMatrix.at<double>(0,2) << cameraMatrix.at<double>(0,0);
  //  qDebug() << cameraMatrix.at<double>(1,2) << cameraMatrix.at<double>(1,1);
    const double fx = cameraMatrix.at<double>(0,0);
    const double fy = cameraMatrix.at<double>(1,1);
    const double cx = cameraMatrix.at<double>(0,2);
    const double cy = cameraMatrix.at<double>(1,2);
    double x = (coord.x - cx)/fx;
    double y = (coord.y - cy)/fy;
   /* if(!useintistics)
    {
        x = coord.x;
        y = coord.y;
    }*/
    double k1 = distCoef.at<double>(0,0);
    double k2 = distCoef.at<double>(0,1);
    double p1 = distCoef.at<double>(0,2);
    double p2 = distCoef.at<double>(0,3);
    if(distcoef.data)
    {
        k1 = distcoef.at<double>(0,0);
        k2 = distcoef.at<double>(0,1);
        p1 = distcoef.at<double>(0,2);
        p2 = distcoef.at<double>(0,3);
    }
    qDebug()<<"undistManual" << k1 << k2 << p1 << p2;


    double xy = x*y;
    double x2 = x*x;
    double y2 = y*y;
    double r = x2+y2;
    double moj = 1+k1*r+k2*r*r;

    Point2f out;
    out.x = x*moj+2*p1*xy+p2*(3*x2+y2);
    out.y = y*moj+p1*(x2+3*y2)+2*p2*xy;
  /*  if(!useintistics)
    {*/
        out.x = out.x*fx+cx;
        out.y = out.y*fy+cy;
   // }
    return out;
}

void Calibration::saveDebug(std::string fname)
{
    cv::FileStorage fs(fname,FileStorage::WRITE);

    Mat x,y;
    x.create(Size(detected_corners.size(),model_corners.size()),CV_32FC1);
    y.create(Size(detected_corners.size(),model_corners.size()),CV_32FC1);

    for(unsigned int i=0;i<detected_corners.size();i++)
    {
        for(unsigned int j=0;j<model_corners.size();j++)
        {
            x.at<float>(i,j)=detected_corners.at(i).at(j).x;
            y.at<float>(i,j)=detected_corners.at(i).at(j).y;
        }
    }
    fs << "Cam" << cameraMatrix;
    fs << "Dist" << distCoef;
    fs << "x" << x;
    fs << "y" << y;
}

cv::Size Calibration::getGridSize()
{
    return    pattern_size;
}
void Calibration::generateObject()
{
    model_corners.clear();
    for(int i=0;i<pattern_size.height;i++)
    {
        for(int j=0;j<pattern_size.width;j++)
         {
                cv::Point3f pt = cv::Point3f(i*squareSize,j*squareSize,0);
                model_corners.push_back(pt);
         }

   }
}

void Calibration::setGridSize(cv::Size sz)
{
    if(sz.width>0&&sz.height>0)
    {
        pattern_size = sz;
        generateObject();
    }
}

double Calibration::getSquereSize()
{
    return squareSize;
}

void Calibration::setSquereSize(double value)
{
    if(value>0){
        squareSize = value;
        generateObject();
    }
}

Mat Calibration::calcInverted(const unsigned int width,const unsigned int height,const unsigned int l)
{

    /* Р’С‹С‡РёСЃР»РµРЅРёРµ РѕР±СЂР°С‚РЅРѕРіРѕ РїСЂРµРѕР±СЂР°Р·РѕРІР°РЅРёСЏ РіРµРѕРјРµСЂС‚РёРё РєР°РјРµСЂС‹
      РёС‰РµС‚СЃСЏ СЂРµС‰РµРЅРёРµ РІ РІРёРґРµ РІРµРєС‚РѕСЂР° q= [l1,l2,b1,b2]
      */
    const double fx = cameraMatrix.at<double>(0,0);
    const double fy = cameraMatrix.at<double>(1,1);
    const double cx = cameraMatrix.at<double>(0,2);
    const double cy = cameraMatrix.at<double>(1,2);
    const double k1 = distCoef.at<double>(0,0);
    const double k2 = distCoef.at<double>(0,1);
    const double p1 = distCoef.at<double>(0,2);
    const double p2 = distCoef.at<double>(0,3);

    //qDebug() << fx << fy << cx << cy;
    //qDebug() << k1 << k2 << p1 << p2;
    Mat A,v;
    double stepx = ceil((double)width/(double)l);


    double stepy = ceil((double)height/(double)l);
    A.create(Size(4,4),CV_32FC1);
    A.setTo(0);
    Mat a;
    a.create(Size(1,4),CV_32FC1);
    a.setTo(0);
    v.create(Size(1,4),CV_32FC1);
    v.setTo(0);
    typedef float mytype;
    //РЎС‚СЂРѕРёРј СЃРµС‚РєСѓ РёР· l РЅР° l С‚РѕС‡РµРє РїРѕРєСЂС‹РІР°СЋС‰СѓСЋ РІСЃРµ РёР·РѕР±СЂР°Р¶РµРЅРёРµ
    for(unsigned int i=0;i<width;i+=stepx)
    {
        for(unsigned int j=0;j<height;j+=stepy)
        {
        //Р’С‹С‡РёСЃР»СЏРµРј x,y СЃРјРµС‰РµРЅРЅС‹Рµ РЅР° С†РµРЅС‚СЂ РєР°РјРµСЂС‹ Рё РґРµР»РµРЅРЅС‹Р№ РЅР° С„РѕРєСѓСЃРЅС‹Рµ СЂР°СЃС‚РѕСЏРЅРёСЏ
        const double x = (i - cx)/fx;
        const double y = (j - cy)/fy;

        const double xy = x*y;

        const double r = x*x+y*y;
        const double moj = 1+k1*r+k2*r*r;

        //Р”РѕР±Р°РІР»СЏРµРј РґРёСЃС‚РѕСЂСЃРёСЋ
        const double nx = x*moj+2*p1*xy+p2*(3*x*x+y*y);
        const double ny = y*moj+p1*(x*x+3*y*y)+2*p2*xy;


        const double rc = nx*nx+ny*ny;

        //РЎРѕСЃС‚Р°РІР»СЏРµРј РѕРіСЂРѕРјРЅСѓСЋ РјР°С‚СЂРёС†Сѓ РІ СѓР·Р»Р°С… РґР»СЏ РєР°Р¶РґРѕР№ С‚РѕС‡РєРё x,y СЃС‚РѕРёС‚ РёСЃРєСЂРёРІР»РµРЅРЅС‹Р№
        a.at<mytype>(0,0) = nx*rc;
        a.at<mytype>(1,0) = nx*rc*rc;
        a.at<mytype>(2,0) = 2*nx*ny;
        a.at<mytype>(3,0) = 3*nx*nx + ny*ny;

        A = A + a * a.t();
        v = v + a * (x - nx);

        a.at<mytype>(0,0) = ny*rc;
        a.at<mytype>(1,0) = ny*rc*rc;
        a.at<mytype>(2,0) = nx*nx + 3*ny*ny;
        a.at<mytype>(3,0) = 2*nx*ny;

        A = A + a * a.t();
        v = v + a * (y - ny);
        }
    }

    Mat q;
    q.create(Size(1,5),CV_32FC1);
    q.setTo(0.f);
    q = A.inv()*v;
    q.convertTo(q,CV_64FC1);

 //   qDebug() << "q="<<q.cols << q.rows << ";" << q.at<float>(0,0) << q.at<float>(0,1) << q.at<float>(0,1) << q.at<float>(0,3);
    return q;

}
double Calibration::computeReprojectionErrors( const vector<vector<Point3f> >& objectPoints,
                          const vector<vector<Point2f> >& imagePoints,
                          const vector<Mat>& rvecs, const vector<Mat>& tvecs,
                          const Mat& cameraMatrix , const Mat& distCoeffs,
                          vector<float>& perViewErrors)
{
    vector<Point2f> imagePoints2;
    int i, totalPoints = 0;
    double totalErr = 0, err;
    perViewErrors.resize(objectPoints.size());

    for( i = 0; i < (int)objectPoints.size(); ++i )
    {
      cv::projectPoints( Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,  // project
                                           distCoeffs, imagePoints2);
      err = norm(Mat(imagePoints[i]), Mat(imagePoints2));              // difference

      int n = (int)objectPoints[i].size();
      perViewErrors[i] = (float) std::sqrt(err*err/n);                        // save for this view
      totalErr        += err*err;                                             // sum it up
      totalPoints     += n;
    }

    return std::sqrt(totalErr/totalPoints);              // calculate the arithmetical mean
}
Calibration::~Calibration()
{
}
 void Calibration::initUndistort()
 {
     //Undistort map
     Mat newCamera;
     if(!getCamMatrix().data||!getDistCoef().data)
         return;


     dx.create(imsize.height, imsize.width, CV_16SC2);
     dy.create(imsize.height, imsize.width, CV_16UC1);

     idx.create(imsize.height, imsize.width, CV_16SC2);
     idy.create(imsize.height, imsize.width, CV_16UC1);

     Mat_<double> Ar, I = Mat_<double>::eye(3,3);
     getCamMatrix().copyTo(Ar);

     Mat dist = getDistCoef();
   //  qDebug() << "Direct" << dist.at<double>(0,0) << dist.at<double>(0,1) << dist.at<double>(0,2) << dist.at<double>(0,3) << dist.at<double>(0,4);

     cv::initUndistortRectifyMap(getCamMatrix(),
                                 dist,
                                 I,Ar,imsize,
                                 dx.type(),dx,dy);
     //Inverted distortion
     Mat inv = calcInverted(imsize.width,imsize.height,10);
     //Inverted distortion map
     Mat out;
     I = Mat_<double>::eye(3,3);
     getCamMatrix().copyTo(Ar);
  //   qDebug() << "Inverted" << inv.at<double>(0,0) << inv.at<double>(0,1) << inv.at<double>(0,2) << inv.at<double>(0,3) << inv.at<double>(0,4);
     cv::initUndistortRectifyMap(getCamMatrix(), inv,
                                     I,Ar,imsize,
                                     idx.type(),idx,idy);

 }

Mat Calibration::loadDebug(std::string fname)
{
    cv::FileStorage fs(fname,FileStorage::READ);
    Mat x,y;
    fs ["Cam"] >> cameraMatrix;
    fs ["Dist"] >> distCoef;
    fs ["x"] >> x;
    fs ["y"] >> y;

    for(int i=0;i<x.cols;i++)
    {
        std::vector<Point2f> vect;
        for(int j=0;j<x.rows;j++)
        {
            Point2f pt;
            pt.x = x.at<float>(i,j);
            pt.y = y.at<float>(i,j);
            vect.push_back(pt);
        }
        detected_corners.push_back(vect);
    }
    Mat coord;
    Mat xx = x.t();
    Mat yy = y.t();
    std::vector<Mat> m;
    m.push_back(x.reshape(0,1));
    m.push_back(y.reshape(0,1));
    cv::merge(m,coord);
    initUndistort();
    return coord;
}
