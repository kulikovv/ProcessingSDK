#include "kinectrgb.h"
#include <types/rgbd.h>
#include <visual/forms/simpleimageview.h>

using namespace visual;
using namespace visual::kinect;

KinectRGB::KinectRGB(QObject *parent)
    : visual::ImageVisualNode(parent)
{
    //TODO: Initiation source
    visual::VisualNode::_widget = new ui::SimpleImageView();
}

void KinectRGB::receiveProcess(esp data)
{
    QSharedPointer<types::Rgbd> frame = data.getReadOnly<types::Rgbd>();
    if (frame)
        qobject_cast<ui::SimpleImageView *>(visual::VisualNode::_widget)->setData(visual::ImageVisualNode::toQImage(frame->rgb()));
}
