#ifndef VIDEOOUTPUTTEST_H
#define VIDEOOUTPUTTEST_H

#include "../../../AutoTest.h"
#include <opencv2/core/core.hpp>

#include <io/media/videooutput.h>
#include <io/media/videosource.h>

class VideoOutputTest : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void saveingTest();
    void cleanupTestCase();
    void loadingTest();
    void drawObject(cv::Mat img,cv::Point2f coord);
signals:
    void send(esp ptr);
private:
    io::media::VideoOutput* _output;
    io::media::VideoSource* _source;
};
DECLARE_TEST(VideoOutputTest)
#endif // VIDEOOUTPUTTEST_H
