#include "fpslistener.h"
#include <opencv2/core/core.hpp>

using namespace process;

FpsListener::FpsListener(QObject *parent) :
    ProcessNode(parent)
{
    setEnable(true);
    _lasttime = cv::getTickCount();
    _fps = 0;
}
double FpsListener::fps(){
    return _fps;
}

void FpsListener::receiveProcess(esp data)
{
  //  qint64 ticks = _timer.elapsed();
    double newtime =  cv::getTickCount();
    double ellapsed= (newtime - _lasttime) /cv::getTickFrequency();
    if(ellapsed>0)
    {
        _fps = 1/ellapsed;
    }
   _lasttime = newtime;
   emit sendFps(_fps);
}
