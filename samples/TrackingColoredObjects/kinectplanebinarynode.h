#ifndef KinectPlaneBinaryNode_H
#define KinectPlaneBinaryNode_H

#include <process/binarynode.h>

struct Plane{
	cv::Point3f pt; //Point
	cv::Point3f n; //Normal
	float norm;
	float distance(cv::Point3f p);
};

struct Cylinder{
    cv::Point3f pt; //Point
    cv::Point3f n; //Direction
    float r;
    float distance(cv::Point3f p);
};

class KinectPlaneBinaryNode :
	public process::BinaryNode
{
	Q_OBJECT
    /*! Порог по растоянию
     */
	Q_PROPERTY( double minMeters READ minMeters WRITE setMinMeters  )
    Q_PROPERTY( int median READ median WRITE setMedian  )
public:
	KinectPlaneBinaryNode(QObject* parent);
	double minMeters();
    int median(){return _median;}
public slots:
	void	setMinMeters(double value);
    void setMedian(int value);
    virtual void reset();
protected:
	//build plane from point set
	bool buildPlane(std::vector<cv::Point3f> points,Plane &bmyplane);
	//find biggest plane
    bool planeRansac(cv::Mat cloud, Plane &myplane, int niter=100);
	virtual void receiveProcess(esp data);
private:
    cv::Mat _cloud;//!< Облако точек
    Plane _realplane;//! Параметры плоскости
    bool _needtobuild;//!
    double _meterth;//!
    int _median;//!< Медианное значение

};

#endif
