#include "rgb.h"
#include <QImage>
#include <opencv2/imgproc/imgproc.hpp>
#include <utils/logs.h>

using namespace types;

Rgb::Rgb()
{
}

Rgb::Rgb(const cv::Mat &x)
    : _mat(x)
{
}

Rgb::Rgb(const Rgb &another)
    : _mat(another._mat)
{
}

Rgb::Rgb(const QImage &image)
{
    if(24==image.depth())
    {
        _mat.create(cv::Size(image.width(),image.height()),CV_8UC3);
        memcpy(_mat.data ,image.bits(),_mat.size[0]);
    }
    if(32==image.depth())
    {
        _mat.create(cv::Size(image.width(),image.height()),CV_8UC3);

        int w = image.width();
        for (int y=0;y<image.height();y++)
        {
           const uchar* line = image.scanLine(y);
           uchar* pmatline = _mat.ptr(y);
           for (int x=0;x<w;x++)
           {
               pmatline[3*x] = line[4*x];
               pmatline[3*x+1] = line[4*x+1];
               pmatline[3*x+2] = line[4*x+2];
           }
        }
    }
}

Rgb &Rgb::operator=(const cv::Mat &x)
{
    _mat = x;
    return *this;
}

Rgb &Rgb::operator=(const Rgb &another)
{
    _mat = another._mat;
    return *this;
}

Rgb::operator cv::Mat() const
{
    return _mat;
}

cv::Mat Rgb::getMat() const
{
    return _mat;
}

cv::Mat Rgb::toGray() const
{
    if (1 == _mat.channels())
        return _mat;
    if(!_mat.data){
        WARNING("Try to convert empty matrix")
        return _mat;
    }

    cv::Mat gray;
    cv::cvtColor(_mat, gray, cv::COLOR_RGB2GRAY);
    return gray;
}

cv::Mat Rgb::toColor() const
{
    return createColor(getMat());
}

cv::Mat Rgb::createColor(const cv::Mat &x)
{
    cv::Mat color;
    if (3 == x.channels())
      //  cv::cvtColor(x, color, cv::COLOR_BGR2RGB);
        return x;
    if (!x.data){
        WARNING("Try to convert empty matrix")
        return x;
    }
    cv::Mat norm;
    if(CV_8UC1!=x.type())
    {
        cv::normalize(x, norm, 255, 0, cv::NORM_MINMAX, CV_8UC1);
    }else{
        norm = x;
    }

    cv::cvtColor(norm, color, cv::COLOR_GRAY2RGB);
    return color;
}


QImage* Rgb::createQImage(QImage* qim)
{
    if(!_mat.data)
     return 0;
    cv::Mat color;
    //Если один канал у изображения
    color = createColor(_mat);

    QImage*  pImage=0;
    if(0==qim)
    {
        pImage = new QImage((const uchar *) color.data,color.cols,color.rows,color.step,QImage::Format_RGB888);

    }else{
        if((qim->width()==color.cols)&&(qim->height()==color.rows))
        {
            pImage = qim;
            memcpy(pImage->bits(),color.data,color.cols*color.rows*3);
        }else{
            delete qim;
            pImage = new QImage((const uchar *) color.data,color.cols,color.rows,color.step,QImage::Format_RGB888);
        }
    }


    pImage->bits();
   // QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
  //  dest.bits(); // enforce deep copy, see documentation

  //  IplImage im = (IplImage)color;

  //  color.step1()

  //  IplImage im = (IplImage)color;
    //По неизвестным причинам pImage->bytesPerLine() и im.widthStep могут не совпадать
   /* if(pImage->bytesPerLine()!=color.step1())
    {
        //Копирование по строкам
        for(int i=0;i<pImage->height();i++)
        {
            memcpy(pImage->bits()+i*pImage->bytesPerLine(),color.data+im.widthStep*i,im.widthStep);
        }
    }else{
        //Копирование всего изображения
        memcpy(pImage->bits(),color.data,color.size);
    }*/
    return pImage;
}
