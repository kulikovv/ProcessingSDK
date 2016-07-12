#include "colordetector.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <types/rgbd.h>
#include <QDebug>
#include "Track.h"
#include <types/general_tracking.h>

ColorDetector::ColorDetector(QObject *parent) :
    process::ProcessNode(parent)
{
    _minpoints = 1000;
    _tracker = new Tracker<cv::Point3d>(0.2,0.1,15);
}
ColorDetector::~ColorDetector()
{
    delete _tracker;
}

int ColorDetector::minPoints()
{
    return _minpoints;
}

void ColorDetector::setMinPoints(int value)
{
    _minpoints = value;
}

void ColorHist::buildHist(cv::Mat image)
{
    cv::Vec3b* ptr = (cv::Vec3b*)image.data;
    int ncube2 = _ncube*_ncube;
    for(int i=0;i<image.cols*image.rows;i++)
    {
        _bins[ptr[i][0]/_cubesize+(ptr[i][1]/_cubesize)*_ncube+(ptr[i][2]/_cubesize)*ncube2]++;
    }
}

std::vector<cv::Vec3b> ColorHist::getValuableColors(int thresh)
{
    std::vector<cv::Vec3b> colors;
    int ncube2 = _ncube*_ncube;
    for(int i=0;i<_bins.size();i++)
    {
        if(_bins[i]>thresh)
        {
            int r = (i%_ncube)*_cubesize+_cubesize/2;
            int g = ((i/_ncube)%_ncube)*_cubesize+_cubesize/2;
            int b =  ((i/ncube2)%_ncube)*_cubesize+_cubesize/2;
            if(!(r==b&&b==g))
            {
                colors.push_back(cv::Vec3b(r,g,b));
            }
        }
    }
    return colors;
}
QMap<int,ColoredObject> ColorDetector::detectContourObjects(cv::Mat image,cv::Mat &imageout)
{
    QMap<int,ColoredObject> objects;//List of preallocaed objects with simple params
    std::vector<std::vector<cv::Point2i> > contour;//Container for contours
    std::vector<cv::Vec4i> hirarchy;//Contour hirarchy
    cv::Mat bin;
    cv::cvtColor(image,bin,cv::COLOR_RGB2GRAY);
    cv::threshold(bin,bin,1,255,cv::THRESH_BINARY);
    imageout = bin;

    cv::findContours(bin,contour,hirarchy,cv::RETR_CCOMP,cv::CHAIN_APPROX_SIMPLE);//Detection of all contours on binary image
   // indexes.create(binary.rows,binary.cols,CV_8UC2);
   // indexes.setTo(0);
    int o=0;

    for(unsigned int i=0;i<contour.size();i++)
                {

                    int size = cv::contourArea(contour[i]);

                    //проверка на неинтересные объекты шум и внутенности
                    if(size>_minpoints)
                    {
                        cv::RotatedRect rect = cv::fitEllipse(contour[i]);
                      //  cv::drawContours(indexes,contour,i,cv::Scalar(o%256,o/256,0),CV_FILLED);
                        ColoredObject obj;
                        obj.count = size;
                        obj.point = rect.center;
                        cv::Point2i c = cv::Point2i((int)rect.center.x,(int)rect.center.y);
                        if(c.x>=0&&c.x<image.cols&&c.y>=0&&c.y<image.rows)
                        {
                            obj.color = image.at<cv::Vec3b>(c);
                            objects.insert(o,obj);
                            o++;
                        }
                    }

                }
    qDebug() << "Found objects" << o;
    return objects;

}

QMap<int, ColoredObject> ColorDetector::detectColoredObjectsOnImage(cv::Mat image,cv::Mat &imageout)
{

    imageout =  image.clone();
    imageout.setTo(0);
    QMap<int, ColoredObject> result;
    ColorHist hist;
    hist.buildHist(image);
    std::vector<cv::Vec3b> colors = hist.getValuableColors(_minpoints);
    for(int i=0;i<colors.size();i++)
    {
        qDebug() << colors[i][0]<< colors[i][1]<< colors[i][2];
        result.insert(colors[i][0]+colors[i][1]*256+colors[i][2]*65536,ColoredObject());
    }

    for(int i=0;i<image.rows;i++)
    {
        for(int j=0;j<image.cols;j++)
        {
            cv::Vec3f b(image.at<cv::Vec3b>(i,j));
            if(b!=cv::Vec3f(0,0,0))
            {
                double min=9999;
                int minid=-1;
                for(int c=0;c<colors.size();c++)
                {
                    cv::Vec3f a(colors[c]);
                    double w = cv::norm(a-b);
                    if(w<min)
                    {
                        minid = c;
                        min = w;
                    }
                }
                if(-1!=minid){
                    imageout.at<cv::Vec3b>(i,j) = colors[minid];
                    result[colors[minid][0]+colors[minid][1]*256+colors[minid][2]*65536].addPoint(cv::Point2f(j,i));
                }
            }
        }
    }
    return result;
}

void ColorDetector::receiveProcess(esp data)
{
    QSharedPointer<types::Rgbd> cloud = data.getReadOnly<types::Rgbd>();
    cv::Mat res;
    if(!cloud.isNull())
    {
        qDebug() << "Detect";
        QMap<int, ColoredObject> x= detectContourObjects(cloud->rgb().getMat(),res);
        std::vector<cv::Point3d> pts;
        res = cloud->rgb().getMat().clone();
        foreach(ColoredObject obj,x.values())
        {

            cv::circle(res,obj.point,5,cv::Scalar::all(255),2);
            cv::Point3f p = cloud->getPointAt(cloud->depth().getMat(),obj.point);
            pts.push_back(p);
        }
        _tracker->addSamples(pts);
        std::vector<cv::Point3d> npt;
        std::vector<cv::Point3d> rpt;
        std::vector<int> rids;
        qDebug() << "Track";
        types::TrackedObjects obj = _tracker->track(npt,rids,rpt);
        qDebug() << "Track end";
        SEND(types::TrackedObjects,new types::TrackedObjects(obj))
    }

    QSharedPointer<types::Rgb> image = data.getReadOnly<types::Rgb>();
    if(!image.isNull())
    {
       QMap<int, ColoredObject> x= detectColoredObjectsOnImage(image->getMat(),res);
       std::vector<cv::Point3d> pts;
       foreach(ColoredObject obj,x.values())
       {
           cv::circle(res,obj.point,5,cv::Scalar::all(255),2);
           pts.push_back(cv::Point3d(obj.point.x,obj.point.y,0));
       }
       _tracker->addSamples(pts);
       std::vector<cv::Point3d> npt;
       std::vector<cv::Point3d> rpt;
       std::vector<int> rids;
       types::TrackedObjects obj = _tracker->track(npt,rids,rpt);
       SEND(types::TrackedObjects,new types::TrackedObjects(obj))
    }
    SEND(types::Rgb,new types::Rgb(res))
}
