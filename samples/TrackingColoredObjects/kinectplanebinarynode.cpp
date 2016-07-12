#include "kinectplanebinarynode.h"
#include <types/Rgbd.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <time.h>
#include <utils/logs.h>
#include <types/multimap.h>
using namespace types;


KinectPlaneBinaryNode::KinectPlaneBinaryNode(QObject* parent):process::BinaryNode(parent)
{
	_needtobuild = true;
	_meterth = 0.05f;
    _median = 0;
}
void KinectPlaneBinaryNode::reset()
{
    _needtobuild = true;
}

inline float Plane::distance(cv::Point3f p)
{
	return fabs(n.x*(pt.x-p.x)+n.y*(pt.y-p.y)+n.z*(pt.z-p.z))/norm;
}

inline float Cylinder::distance(cv::Point3f p)
{
    return 0;
}

bool KinectPlaneBinaryNode::buildPlane(std::vector<cv::Point3f> points,Plane &bmyplane)
{
	if(3!=points.size())
	{
		return false;
	}

	if(points[2]==points[0]||points[1]==points[2]||points[0]==points[1])
	{
		return false;
	}
	cv::Point3f a,b;

	a.x=points[2].x-points[0].x;
	a.y=points[2].y-points[0].y;
	a.z=points[2].z-points[0].z;
	
	
	b.x=points[1].x-points[0].x;
	b.y=points[1].y-points[0].y;
	b.z=points[1].z-points[0].z;
	
	
	bmyplane.n.x=(a.y*b.z)-(a.z*b.y);
	bmyplane.n.y=(a.z*b.x)-(a.x*b.z);
	bmyplane.n.z=(a.x*b.y)-(a.y*b.x);

	bmyplane.pt = points[0];
	bmyplane.norm = sqrt(bmyplane.n.x*bmyplane.n.x+bmyplane.n.y*bmyplane.n.y+bmyplane.n.z*bmyplane.n.z);
	if(0==bmyplane.norm){
		return false;
	}
	return true;
}

//find biggest plane
bool KinectPlaneBinaryNode::planeRansac(cv::Mat cloud,Plane &bmyplane,int niter)
{

	Plane bestplane;
	float inliners = 0;
	float th=0.01f;

    std::vector<cv::Point3f> inlinersVector;
    std::vector<cv::Point3f> outliersVector;

	std::vector<cv::Point3f> fullset;

	for(int i=0;i<cloud.rows;i=i+5)
    {
        for(int j=0;j<cloud.cols;j=j+5)
        {
			cv::Point3f value = cloud.at<cv::Point3f>(i,j);
            if(0!=value.x)
            {
				fullset.push_back(value);
            }
		}
	}
	if(fullset.size()<3){
		return false;
	}
	srand( clock());
	
    for(int l=0;l<niter;l++)
	{
		int inl=0;
		std::vector<cv::Point3f> pts;
		for(int i=0;i<3;i++)
		{
			int idx = rand()%fullset.size();
			pts.push_back(fullset[idx]);
		}
		Plane tmpplane;
        std::vector<cv::Point3f> tmpInliners;
        std::vector<cv::Point3f> tmpOutliers;
		if(buildPlane(pts,tmpplane))
		{
			for(unsigned int i=0;i<fullset.size();i++)
			{
				if(tmpplane.distance(fullset[i])<th)
				{
					inl++;
                    tmpInliners.push_back(fullset[i]);
                }else{
                    tmpOutliers.push_back(fullset[i]);
                }
			}
			if(inl>inliners)
			{
				inliners = inl;
				bestplane = tmpplane;
                inlinersVector = tmpInliners;
                outliersVector = tmpOutliers;
			}
		}
	}

	bmyplane = bestplane;
	return true;
}

double KinectPlaneBinaryNode::minMeters(){
	return _meterth;
}
void KinectPlaneBinaryNode::setMinMeters(double value)
{
	_meterth = value;
}
void KinectPlaneBinaryNode::setMedian(int value)
{
    if(_median >=0 && _median <= 7)
    {
        _median = value;
    }
}

void KinectPlaneBinaryNode::receiveProcess(esp data)
{

    if(data=="HOOK")
    {
        emit send(data);
        return;
    }


    QSharedPointer<Rgb> readonly = data.getReadOnly<Rgb>();
	QSharedPointer<Rgbd> cloud = data.getReadOnly<Rgbd>();
    if(data=="MASK"&&!readonly.isNull())
    {
		_mask = readonly->getMat().clone();
        if(1!=_mask.channels())
        {
            cv::Mat graymask;
            cv::cvtColor(_mask,graymask,cv::COLOR_RGB2GRAY);
			_mask = graymask;
        }
        _mask=255-_mask;

       // emit send(data);
		_needtobuild = true;
        return;
    }
    if(!cloud.isNull())
	{
       // _cloud = cloud->depth().getMat();
        cloud->getPointCloud(_cloud);
        if(_cloud.empty())
        {
         //   _cloud = cloud->depth().getMat();
            return;
        }

        cv::Mat mycloud = _cloud;


		if(!_mask.empty()){
            if(_mask.cols!=mycloud.cols||_mask.rows!=mycloud.rows){
                _mask=_mask(cv::Rect(0,0,mycloud.cols,mycloud.rows));
            }
            mycloud.setTo(cv::Scalar::all(0),_mask);
		}

		if(_needtobuild)
		{
			if(planeRansac(mycloud,_realplane))
			{
				_needtobuild = false;
			}else{
				return;
			}
		}
		if(!_needtobuild)
		{		
            cv::Mat gray(mycloud.rows,mycloud.cols,CV_32FC3,cv::Scalar::all(0));

			cv::Mat binary(mycloud.rows,mycloud.cols,CV_8UC3,cv::Scalar::all(0));
            cv::Mat rgb = cloud->rgb().getMat() ;
           /* if(0!=_median){
                cv::medianBlur(mycloud,mycloud,_median);
            }*/

            cv::Mat_<cv::Point3f> mycloud_ = mycloud;
            cv::Mat_<cv::Point3f> gray_ = gray;
            cv::Mat_<cv::Vec3b> binary_ = binary;

			for(int i=0;i<mycloud.rows;i++)
			{
				for(int j=0;j<mycloud.cols;j++)
				{
                    cv::Point3f value = mycloud_(i,j);
					if(0!=value.x)
					{					
						float dist =_realplane.distance(value);		
                        gray_(i,j)=cv::Point3f(value.y,value.z,dist);
						if(dist>_meterth){

                            binary_(i,j) = rgb.at<cv::Vec3b>(i,j)+cv::Vec3b(1,1,1);// cv::Vec3b(255,255,255);
						}
					}
				}
			}



            types::MultiMap *mm=new types::MultiMap();
            mm->operator []("RGB")=cloud->rgb().getMat();
            mm->operator []("Cloud")=gray;
            mm->operator []("Binary")=binary;
            SEND(MultiMap,mm)
            //SEND(Rgb,new Rgb(binary))
            //qDebug() << "Send" << frame;

            SEND(Rgbd,new Rgbd("",binary,gray,cv::Point3d(0,0,0)))
		}
	}

}
