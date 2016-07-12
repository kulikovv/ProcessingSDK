#include "videoinputcapture.h"
#include <utils/logs.h>
#include <types/rgb.h>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>


using namespace io;
using namespace io::capture;
using namespace types;

VideoInputCapture::VideoInputCapture(QObject *parent) :
    SingleCameraNode(parent)
{
}

esp VideoInputCapture::get(const unsigned long frame)
{
    esp pointer;
    return pointer;
}

bool VideoInputCapture::isConnected()
{
    return false;
}

void VideoInputCapture::setDeviceid (int value)
{

}


bool VideoInputCapture::getFrame(cv::Mat &mat)
{
    return false;
}
