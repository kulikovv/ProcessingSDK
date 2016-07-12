#include "videooutputtest.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <types/rgb.h>
#include <QFile>

#include <opencv2/highgui/highgui.hpp>
#define FILENAME "D:/VideoOutputTest.avi"
#define FRAMENUMB 100
void VideoOutputTest::initTestCase()
{
    _output = new io::media::VideoOutput(0);
    _source = new io::media::VideoSource(0);
    if(QFile::exists(FILENAME))
    {
        QFile::remove(FILENAME);
    }
}

void VideoOutputTest::cleanupTestCase()
{
    delete _output;
    delete _source;

}

void VideoOutputTest::drawObject(cv::Mat img,cv::Point2f coord)
{
    img.setTo(0);
    cv::circle(img,coord,10,cv::Scalar::all(255),-1);
    esp data = esp::init<types::Rgb>(new types::Rgb(img));
    emit send(data);
}

void VideoOutputTest::saveingTest()
{
    return;
    _output->setUrl(FILENAME);
    _output->setCompression(io::media::VideoOutput::SelectCodec);
    _output->setFps(25);
    _output->setEnable(true);

    cv::Mat testimg;
    testimg.create(480,640,CV_8UC3);

    connect(this,SIGNAL(send(esp)),_output,SLOT(receive(esp)));

    _output->start();

    for(int i=0;i<FRAMENUMB;i++)
    {
        drawObject(testimg,cv::Point2f(i+50,150));
    }

    _output->stop();


    QVERIFY(QFile::exists(FILENAME));
}

 void VideoOutputTest::loadingTest()
 {
     return;
     QVERIFY(QFile::exists(FILENAME));
     _source->setUrl(FILENAME);
     QCOMPARE((int)_source->lenght(),FRAMENUMB);
     QCOMPARE((int)_source->fps(),25);

     cv::Mat testimg;
     testimg.create(480,640,CV_8UC3);

     disconnect(_output);
     int missing = 0;
     for(int i=_source->lenght()-1;i>=0;i--)
     {
       esp data = _source->get(i);
       QSharedPointer<types::Rgb> sptr = data.getReadOnly<types::Rgb>();
       if(sptr.isNull())
       {
           missing++;
           continue;
       }
     }
     qDebug() << "Missing" << missing;
     missing = 0;
     for(int i=0;i<_source->lenght();i++)
     {
       esp data = _source->get(i);
       QSharedPointer<types::Rgb> sptr = data.getReadOnly<types::Rgb>();
       if(sptr.isNull())
       {
           missing++;
           continue;
       }
       drawObject(testimg,cv::Point2f(i+50,150));
       cv::Mat diff;
       cv::absdiff(testimg, sptr->getMat(),diff);
       cv::Scalar mysum = cv::sum(diff);
     //  qDebug() << i << mysum[0] << mysum[1] << mysum[2];
     }
     qDebug() << "Missing" << missing;
 }
