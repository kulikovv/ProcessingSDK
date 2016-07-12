#ifndef __KINECT_RGB__H__
#define __KINECT_RGB__H__

#include <visual/imagevisualnode.h>
namespace visual
{
namespace kinect
{
class KinectRGB: public visual::ImageVisualNode
{
    Q_OBJECT
    Q_CLASSINFO("Accept", "KinectFrame")

public:
    explicit KinectRGB(QObject *parent = 0);

protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp);

};
}
}
#endif // __KINECT_RGB__H__
