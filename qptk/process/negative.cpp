#include "negative.h"
#include <types/rgb.h>
#include <opencv2/imgproc/imgproc.hpp>
using namespace types;
using namespace process;

Negative::Negative(QObject *parent) :
    ProcessNode(parent)
{
    //TODO: Initiation source
}
Negative::~Negative()
{
    //TODO: Destruction	
}

void Negative::receiveProcess(esp data)
{
    QSharedPointer<Rgb> readonly = data.getReadOnly<Rgb>();

    if(!readonly.isNull())
    {
       cv::Mat workcopy = readonly->getMat().clone();
       cv::morphologyEx(readonly->getMat(),workcopy,cv::MORPH_BLACKHAT,cv::Mat());
       SEND(Rgb,new Rgb(workcopy))
    }
}

