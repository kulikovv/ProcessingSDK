#ifndef PROCESSINGFRAMEWORK_KINECTPLAYNODE_H
#define PROCESSINGFRAMEWORK_KINECTPLAYNODE_H

#include <io/sourcenode.h>

namespace io
{
namespace kinect{
class KinectStorage;
class KinectPlayNode : public SourceNode {
    Q_OBJECT
    Q_CLASSINFO("Send","KinectFrame")
    Q_CLASSINFO("readSerial",tr("<b>minimum=1.0;</b>maximum=10.0;"))
    Q_PROPERTY(bool readSerial READ readSerial WRITE setReadSerial)
public:
    explicit KinectPlayNode(QObject *parent);
    ~KinectPlayNode();
    /*!Call from synchronizer 
      * general rule frame = (0,length())
      * if frame = -1 it means to take next frame
      */
    virtual esp get(const unsigned long frame);
protected:
    /*! Call from other Nodes
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
public slots:
    virtual void setUrl(QString value);
    bool readSerial();
    void setReadSerial(bool rs);
private:
    //Current data
    KinectStorage* ks;
    long postedframes;
    //
    bool _readserial;
};
}
}
#endif // KINECTPLAYNODE_H


