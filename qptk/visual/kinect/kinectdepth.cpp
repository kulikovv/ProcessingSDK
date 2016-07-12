#include "kinectdepth.h"
#include <types/rgbd.h>
#include <visual/forms/simpleimageview.h>



using namespace visual;
using namespace visual::kinect;

KinectDepth::KinectDepth(QObject *parent)
    : visual::ImageVisualNode(parent)
{
    //TODO: Initiation source
    visual::VisualNode::_widget = new ui::SimpleImageView();
}

void KinectDepth::receiveProcess(esp data)
{
    QSharedPointer<types::Rgbd> framex = data.getReadOnly<types::Rgbd>();
    if (!framex.isNull())
    {
        cv::Mat img = framex->depth().getMat();
        types::Rgb data;
        if(img.type()==CV_32FC3){
            cv::Mat eightBit,mulCh;
            std::vector<cv::Mat> channels;
            cv::split(img,channels);
            channels[2].convertTo(eightBit,CV_8UC1,255,0);
            channels.clear();
            channels.push_back(eightBit);
            channels.push_back(eightBit);
            channels.push_back(eightBit);
            cv::merge(channels,mulCh);
            data = types::Rgb(mulCh);

            SEND(types::Rgb,new types::Rgb(mulCh))
        }else{
            data= framex->depth();
        }
        qobject_cast<ui::SimpleImageView *>(visual::VisualNode::_widget)->setData(visual::ImageVisualNode::toQImage(data));
    }
}
