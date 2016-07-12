#ifndef __KINECT_DEPTH__H__
#define __KINECT_DEPTH__H__

#include <visual/imagevisualnode.h>
namespace visual
{
/*! \brief Визуализация данных типа Rgbd
    Классы для визуализации дальностных данных.
  */
namespace kinect
{
class KinectDepth: public visual::ImageVisualNode
{
    Q_OBJECT
    Q_CLASSINFO("Accept", "KinectFrame")

public:
    explicit KinectDepth(QObject *parent = 0);

protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp);

};
}
}

#endif // __KINECT_DEPTH__H__
