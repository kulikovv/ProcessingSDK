#include "depthtracker.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <types/rgbd.h>
#include <types/multimap.h>
#include <math.h>

#define PI 3.14

bool compaire (std::vector<cv::Point> i,std::vector<cv::Point> j) { return (i.size()>j.size()); }

cv::Vec3b DepthTracker::randomColor( cv::RNG& rng )
  {
  int icolor = (unsigned) rng.next();
  return cv::Vec3b( icolor&255, (icolor>>8)&255, (icolor>>16)&255 );
  }

cv::Point DepthTracker::getCenterMass(std::vector<cv::Point> &data)
{
    cv::RotatedRect rect = cv::fitEllipse(data);
    return rect.center;
}

ColorTracker::ColorTracker(QObject *parent):DepthTracker(parent)
{

}

template<typename T> double getStd(std::vector<T> values)
{
    if(values.size()==0){
        return -1;
    }
    T temp;
    temp=0;
    for(unsigned int i=0;i<values.size();i++)
    {
        temp+=values[i];
    }
    double mean = (double)temp/(double)values.size();
    temp=0;
    for(unsigned int i=0;i<values.size();i++)
    {
        temp+=(values[i]-mean)*(values[i]-mean);
    }
    return sqrt((double)temp/(double)values.size());

}



DepthTracker::DepthTracker(QObject *parent) :
    process::ProcessNode(parent)
{
    smin=9;
    smax=17;
    cv::RNG rng( 0xFFFFFFFF);
    for(int i=0;i<100;i++){
        colors.push_back(randomColor(rng));
    }
}

std::vector<types::Object3d> DepthTracker::getCentersMass(cv::Mat labelimage,int n,std::vector<int> &sizes,cv::Mat color,cv::Mat depth){

      std::vector<types::Object3d> pts;

      std::vector<int> cc;
      std::vector<int> cccolor;

      std::vector<cv::Vec3f> meanvalue;

      std::vector<cv::Point3f> dir; //Вычисление направления по черным точкам
      std::vector<cv::Point3f> center;//Список оценок центров масс в физических координатах

      for(int i=0;i<n;i++){
          pts.push_back(types::Object3d());
          cc.push_back(0);
          cccolor.push_back(0);
          meanvalue.push_back(cv::Vec3f());
          dir.push_back(cv::Point3f());
          center.push_back(cv::Point3f());
      }
      cv::Mat_<int> labels = labelimage;
      cv::Mat_<cv::Vec3b> values = color;
      cv::Mat_<cv::Point3f> values_depth = depth;
      for(int i=0;i<labelimage.rows;i++)
      {
          for (int j=0;j<labelimage.cols;j++){
              int v = labels(i,j);
              if(v>1){
                  pts[v-2].center = pts[v-2].center+cv::Point3d(j,i,0);
                  cc[v-2]++;
                  if(!values.empty()&&!values_depth.empty())
                  {
                      meanvalue[v-2]+= values(i,j);
                      center[v-2] = center[v-2] + values_depth(i,j);

                      float gr = float(values(i,j)[0]+values(i,j)[1]+values(i,j)[2])/(3.*255.);
                      if(0!=gr){
                          gr = gr<0.2?1:0;

                          if(gr){
                              dir[v-2]= dir[v-2]+ values_depth(i,j);
                              cccolor[v-2]++;
                          }
                      }

                  }
              }else if(-1==v){
                  //Check neigbours
                  if(i-1>=0)
                  {
                      int v = labels(i-1,j);
                      if(v>1){
                          pts[v-2].contour.push_back(cv::Point2i(j,i-1));
                      }
                  }
                  if(j-1>=0)
                  {
                      int v = labels(i,j-1);
                      if(v>1){
                          pts[v-2].contour.push_back(cv::Point2i(j-1,i));
                      }
                  }
                  if(i+1<labelimage.rows)
                  {
                      int v = labels(i+1,j);
                      if(v>1){
                          pts[v-2].contour.push_back(cv::Point2i(j,i+1));
                      }
                  }
                  if(j+1<labelimage.cols)
                  {
                      int v = labels(i,j+1);
                      if(v>1){
                          pts[v-2].contour.push_back(cv::Point2i(j+1,i));
                      }
                  }
              }

          }
      }
      for(int i=0;i<n;i++){
          pts[i].center= pts[i].center*(1.0f/(float)(cc[i]));
          cv::Vec3f mean = meanvalue[i]*(1.0f/(float)(cc[i]));
          cv::Point3f dirpt = dir[i]*(1.0f/(float)(cccolor[i]));
          cv::Point3f centerpt = center[i]*(1.0f/(float)(cc[i]));
        //  qDebug() << "C Count" << cccolor[i] << cc[i];

          pts[i].params["mean1"]= mean[0];
          pts[i].params["mean2"]= mean[1];
          pts[i].params["mean3"]= mean[2];
          pts[i].params["dirx"]= dirpt.x;
          pts[i].params["diry"]= dirpt.y;
          pts[i].params["dirz"]= dirpt.z;
          pts[i].params["x"]= centerpt.x;
          pts[i].params["y"]= centerpt.y;
          pts[i].params["z"]= centerpt.z;
          pts[i].params["angle"]= 180*atan2(dirpt.y-centerpt.y,dirpt.x-centerpt.x)/PI;
          pts[i].params["size"]= cc[i];
       //   qDebug() << "Dir" << dirpt.x << dirpt.y << "Center" << pts[i].center.x << pts[i].center.y << "Angle" <<pts[i].params["angle"];
          try{
              cv::convexHull(pts[i].contour,pts[i].contour);
          }catch(...){

          }
      }
      sizes = cc;
      return pts;
}

void DepthTracker::reset()
{
    oldpts.clear();
    QString tag = "X%1;Y%1;";
    QString dataheader = "";
    for(int i=0;i<_maxanimals;i++)
    {
        dataheader+=tag.arg(i);
    }
    emit send(esp::init(new QString(dataheader),0,""));
}


void DepthTracker::renderWatershade(cv::Mat watershade,cv::Mat render,std::vector<cv::Vec3b> colors,std::vector<int> indexes)
{
    cv::MatIterator_<cv::Vec3b> it, end;
    cv::MatIterator_<int> it2, end2;
  //  render.setTo(0);
    for( it = render.begin<cv::Vec3b>(), end = render.end<cv::Vec3b>(),it2= watershade.begin<int>(),end2=watershade.end<int>();
         it != end; ++it, ++it2)
     {
        if(0==*it2)
        {
            *it = cv::Vec3b(0,0,0);
        }
        if(1==*it2)
        {
            *it = cv::Vec3b(255,255,255);
        }
        if(*it2>1)
        {
            *it = colors[indexes[*it2-2]];
        }
        if(*it2<0){
            *it2 = 0;
            *it = cv::Vec3b(255,255,255);
        }
     }
}
void DepthTracker::setMinSize(int value)
{
    smin = value;
}

void DepthTracker::setMaxSize(int value)
{
    smax = value;
}

void  DepthTracker::setMaxAnimals(int value)
{
    if(value>0){
        _maxanimals = value;
    }
}

void DepthTracker::receiveProcess(esp data)
{
    QSharedPointer<types::Rgbd> cloud;// = data.getReadOnly<types::Rgbd>();
    QSharedPointer<types::MultiMap> map =  data.getReadOnly<types::MultiMap>();
    if(0==frame){
        reset();
    }
    if(!cloud.isNull()||!map.isNull())
    {
        std::vector<cv::Mat> chanels;
        if(!cloud.isNull()){
            cv::split(cloud->depth().getMat(),chanels);
        }
        if(!map.isNull()){
            cv::split(map->value("Cloud",cv::Mat()),chanels);
        }
        cv::Mat img= chanels[2];

        cv::Mat blured,binx,bin,bin1,bin2,dist;

        cv::Mat watershade;
        cv::Mat render;


        std::vector<std::vector<cv::Point> > countours;
        std::vector<std::vector<cv::Point> > tmpcountours;

        std::vector<cv::Vec4i> hierarchy;

    //     qDebug() << "Do blur!";
        cv::blur(img,blured,cv::Size(5,5));
    //    qDebug() << "Do threshold!"
        if(!cloud.isNull()){
            cv::cvtColor(cloud->rgb().getMat(),binx,cv::COLOR_RGB2GRAY);
        }
        if(!map.isNull()){
            cv::cvtColor(map->value("Binary",cv::Mat()),binx,cv::COLOR_RGB2GRAY);
            _color = map->value("RGB",cv::Mat());
           // cv::Mat invbin;
           // cv::threshold(binx,invbin,1,255,CV_THRESH_BINARY_INV);
           // _color.setTo(cv::Vec3b(255,0,255),invbin);
        }

        //////Debug
     /*   for(int i=0;i<_color.rows;i++)
        {
            for(int j=0;j<_color.cols;j++){
                float gr = float(_color.at<cv::Vec3b>(i,j)[0]+_color.at<cv::Vec3b>(i,j)[1]+_color.at<cv::Vec3b>(i,j)[2])/(3.*255.);
             //   if(0!=gr){
                    gr = gr<0.5?1:0;
                    if(gr)
                        _color.at<cv::Vec3b>(i,j)[0]=255;

              //  }
            }
        }

        SEND_TAG(types::Rgb,new types::Rgb(_color),"DRAWING")*/
                ////


        binx.convertTo(binx,CV_8UC1);
        //Parse distance. too near && to far
        cv::distanceTransform(binx,dist,cv::DIST_L2,5);
        cv::threshold(dist,bin1,smin,255,::cv::THRESH_BINARY);
        cv::threshold(dist,bin2,smax,255,::cv::THRESH_BINARY_INV);
        bin = bin1&bin2;

    //    SEND_TAG(types::Rgb,new types::Rgb(bin),"DRAWING")
   //     return;

        bin.convertTo(bin,CV_8UC1,255,0);

        //Find 3 best candidates
        cv::findContours(bin,tmpcountours,hierarchy,::cv::RETR_CCOMP,cv::CHAIN_APPROX_NONE);

        //Filter inner contours
        std::vector<std::vector<cv::Point> > filtred;
        for(unsigned int i=0;i<tmpcountours.size();i++){
            if ( hierarchy[i][3] == -1 && tmpcountours[i].size()>30) {
                filtred.push_back(tmpcountours[i]);
            }
        }

        //Sort contours
        std::sort (filtred.begin(), filtred.end(), compaire);
        //Select 3 best


        //Do watershade
        watershade = cv::Mat(blured.rows,blured.cols,CV_32SC1);
        watershade.setTo(1);
        watershade.setTo(0,binx);

        cv::Mat watershadeAlter;


        bool connected=true;
        if( filtred.size()<_maxanimals)
        {
            qDebug() <<  frame <<"Circle path";
            if(!oldbin.empty())
            {
                for(unsigned int j=0;j<oldpts.size();j++){
                    cv::circle(watershade,cv::Point2f(oldpts[j].center.x,oldpts[j].center.y),5,cv::Scalar::all(j+2),cv::FILLED);
                }
                connected = false;
            }else{
               qDebug() <<  frame <<"Circle path fail";
                return;
            }
          //
            //
        }else{
            qDebug() << frame << "Filter path";

            if(!oldbin.empty())
            {
                watershadeAlter = watershade.clone();
                for(unsigned int j=0;j<oldpts.size();j++){
                    cv::circle(watershadeAlter,cv::Point2f(oldpts[j].center.x,oldpts[j].center.y),5,cv::Scalar::all(j+2),cv::FILLED);
                }
            }

            for(int i=0;i<_maxanimals;i++){
                countours.push_back(filtred[i]);
            }
            for(unsigned int i=0;i<countours.size();i++){

                cv::drawContours(watershade,countours,i,cv::Scalar(i+2),cv::FILLED);
            }


        }


        cv::threshold(img,img,0.09,1,cv::THRESH_TRUNC);

        img.convertTo(img,CV_8UC1,255,0);
        cv::Mat norm;
        cv::normalize(img, norm, 255, 0, cv::NORM_MINMAX, CV_8UC1);
        cv::cvtColor(norm,img,cv::COLOR_GRAY2BGR);

        _depth = img;

        //Image segmentation
        cv::watershed(getSegmentationMap(),watershade);
        if(!watershadeAlter.empty()){
            cv::watershed(getSegmentationMap(),watershadeAlter);
        }


        oldbin = binx.clone();

        //Get data from segmented image and store it in pts
        std::vector<int> sizes,sizesAlter;

        std::vector<types::Object3d> pts = getCentersMass(watershade,_maxanimals,sizes,_color);
        std::vector<types::Object3d> ptsAlter;
        if(!watershadeAlter.empty()){
             ptsAlter = getCentersMass(watershadeAlter,_maxanimals,sizesAlter,_color);
        }

        double stdFilter = getStd<int>(sizes);
        double stdTracking = getStd<int>(sizesAlter);
        qDebug() << "Std" << stdFilter<< stdTracking;

        //Choose working hypotezis using std
        if(stdTracking>-1){
           // qDebug() << "stdTracking" << sizesAlter[0] << sizesAlter[1];
            if(0!=sizesAlter[0]&&0!=sizesAlter[1])
            {
                if(stdTracking<stdFilter)
                {
                    pts = ptsAlter;
                    watershade = watershadeAlter;
                    connected = false;
                }
            }
        }

        std::vector<int> indexes;//Result indexes by default 1...N
        for(int j=0;j<pts.size();j++){
              indexes.push_back(j+1);
        }


        if(connected){

            oldpts = trackObjects(oldpts,pts,indexes);
        }else{
            oldpts = pts;
        }



        //Render image
        render = img.clone();
        renderWatershade(watershade,render,colors,indexes);

        //Fill the output map
        types::TrackedObjects3d objs;
        types::Data* pDat = new types::Data(QStringList() << "X" << "Y");

        for(int i=0;i<oldpts.size();i++){
            objs[i]=oldpts[i];
            pDat->at("X")=objs[i].center.x;
            pDat->at("Y")=objs[i].center.y;

            //Add lebels
            cv::putText(render,QString("%1").arg(i).toStdString(),cv::Point2f(oldpts[i].center.x,oldpts[i].center.y),cv::FONT_HERSHEY_PLAIN,3,cv::Scalar::all(0),2,8);
        }
        //Send tracked data to display window
        //emit trackedData(objs);
        SEND(types::Data,pDat)
        types::MultiMap *map = new types::MultiMap();
        map->insert("SEG",render);
        map->insert("DEPTH",img);
        SEND(types::MultiMap,map)
       // SEND_TAG(types::Rgb,new types::Rgb(render),"DRAWING")
    }

}

/*
std::vector<types::Object3d> DepthTracker::trackObjects(std::vector<types::Object3d> oldobj,std::vector<types::Object3d> newobj,std::vector<int> &indexes)
{
    std::vector<types::Object3d> ptscomb = newobj;
    std::vector<int> indexescomb=indexes;
    std::vector<types::Object3d> bestcomb = newobj;
    if(!oldobj.empty()){
        double min=9999999;
        do{
            double sum=0;
            for(unsigned int j=0;j<oldobj.size();j++){
                ptscomb[j]=newobj[indexescomb[j]-1];
            }
            for(unsigned int j=0;j<oldobj.size();j++){
                 sum += cv::norm(ptscomb[j].center- oldobj[j].center);
            }
            if(sum<min){
                min = sum;
                bestcomb = ptscomb;
                indexes = indexescomb;
            }
        }while(std::next_permutation(indexescomb.begin(),indexescomb.end()));
    }
    return bestcomb;
}*/

std::vector<types::Object3d> DepthTracker::trackObjects(std::vector<types::Object3d> oldobj,std::vector<types::Object3d> newobj,std::vector<int> &indexes)
{
    std::vector<types::Object3d> ptscomb = newobj;
    std::vector<int> indexescomb=indexes;
    std::vector<types::Object3d> bestcomb = newobj;
    if(!oldobj.empty()){
        double min=9999999;
        do{
            double sum=0;
            for(unsigned int j=0;j<oldobj.size();j++){
                ptscomb[j]=newobj[indexescomb[j]-1];
            }
            for(unsigned int j=0;j<oldobj.size();j++){
                 sum += cv::norm(ptscomb[j].center- oldobj[j].center);
            }
            if(sum<min){
                min = sum;
                bestcomb = ptscomb;
                indexes = indexescomb;
            }
        }while(std::next_permutation(indexescomb.begin(),indexescomb.end()));
    }
    return bestcomb;
}


cv::Mat DepthTracker::getSegmentationMap()
{
   return _depth;
}

std::vector<types::Object3d> ColorTracker::trackObjects(std::vector<types::Object3d> oldobj,std::vector<types::Object3d> newobj,std::vector<int> &indexes)
{
    std::vector<types::Object3d> ptscomb = newobj;
    std::vector<int> indexescomb=indexes;
    std::vector<types::Object3d> bestcomb = newobj;
    if(!oldobj.empty()){
        double min=9999999;
        do{
            double sum=0;
            for(unsigned int j=0;j<oldobj.size();j++){
                ptscomb[j]=newobj[indexescomb[j]-1];
            }
            for(unsigned int j=0;j<oldobj.size();j++){
                 cv::Vec3f a(ptscomb[j].params["mean0"],ptscomb[j].params["mean1"],ptscomb[j].params["mean2"]);
                 cv::Vec3f b(oldobj[j].params["mean0"],oldobj[j].params["mean1"],oldobj[j].params["mean2"]);
                 sum += cv::norm(a-b);
                 sum += cv::norm(ptscomb[j].center- oldobj[j].center);
            }
            if(sum<min){
                min = sum;
                bestcomb = ptscomb;
                indexes = indexescomb;
            }
        }while(std::next_permutation(indexescomb.begin(),indexescomb.end()));
    }
    return bestcomb;
}

cv::Mat ColorTracker::getSegmentationMap()
{
    return _color;
}

MapFilter::MapFilter(QObject *parent):process::ProcessNode(parent)
{
    setEnable(true);
    _filter = "";
}

void MapFilter::setFilter(QString value)
{
    _filter = value;
}

void MapFilter::receiveProcess(esp data)
{
    QSharedPointer<types::MultiMap> map =  data.getReadOnly<types::MultiMap>();
    if(!map.isNull())
    {
        cv::Mat datax = map->value(_filter,cv::Mat());
        if(!datax.empty())
        {
            SEND(types::Rgb,new types::Rgb(datax))
        }
    }
}
