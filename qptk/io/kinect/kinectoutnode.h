#ifndef PROCESSINGFRAMEWORK_KINECTOUTNODE_H
#define PROCESSINGFRAMEWORK_KINECTOUTNODE_H

#include <io/outputnode.h>

namespace io{
/*! \brief Классы работы с данныеми полученными с глубинных камер
  Содержит базовые классы для работы с глубинными камерами и стереокамерами с поддержкой
  \sa types::Rgbd
  */
namespace kinect{
class KinectStorage;

class KinectOutNode : public OutputNode {
    Q_OBJECT
    Q_CLASSINFO("Accept","KinectFrame")
public:
    explicit KinectOutNode(QObject *parent);
    ~KinectOutNode();
protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
public slots:
    virtual void setUrl(QString value);
    virtual void stop();
private:
    //Current data
    KinectStorage *ks;
};
}
}

#endif // KINECTOUTNODE_H


