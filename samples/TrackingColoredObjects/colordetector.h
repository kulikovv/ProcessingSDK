#ifndef COLORDETECTOR_H
#define COLORDETECTOR_H

#include <QObject>
#include <process/processnode.h>
#include <opencv2/core/core.hpp>
#include <QMap>

struct ColoredObject
{
    ColoredObject(){
        point = cv::Point2i(0,0);
        color = cv::Vec3b(0,0,0);
        count = 0;
    }
    inline void addPoint(cv::Point2f v)
    {
        count++;
        float icount = 1.0f/(float)count;
        float ccount = float(count-1)*icount;
        point = point*ccount+v*icount;
    }

    cv::Point2f point;
    cv::Vec3b color;
    int count;
};

class ColorHist
{
public:
    ColorHist(int cubesize=64,int maxvalue=256)
    {
        _ncube = maxvalue/cubesize;
        _cubesize = cubesize;
        _bins.resize(int(pow((double)_ncube,3)),0);
    }
    void buildHist(cv::Mat image);
    std::vector<cv::Vec3b> getValuableColors(int thresh = 255);
private:
    std::vector<int> _bins;
    int _cubesize;
    int _ncube;
};

template <class C> class Tracker;

class ColorDetector : public process::ProcessNode
{
    Q_OBJECT
    Q_PROPERTY( int minpoints READ minPoints WRITE setMinPoints  )
public:
    explicit ColorDetector(QObject *parent = 0);
    ~ColorDetector();
    int minPoints();
signals:
    
public slots:
    void setMinPoints(int value);
protected:
    QMap<int,ColoredObject> detectContourObjects(cv::Mat image,cv::Mat &imageout);
    QMap<int,ColoredObject> detectColoredObjectsOnImage(cv::Mat image,cv::Mat &imageout);
    virtual void receiveProcess(esp data);
    int _minpoints;
    Tracker<cv::Point3d>* _tracker;

  //  std::vector<ColoredObject> _objects;//
  //  QMap<cv::Vec3b,ColoredObject> _asso;
};

#endif // COLORDETECTOR_H
