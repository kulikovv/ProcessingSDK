#ifndef DATAIONODE1_H
#define DATAIONODE1_H

#include <io/inoutnode.h>
#include <types/data.h>
#include <types/general_tracking.h>
#include <vector>

#include <opencv2/core/core.hpp>

namespace io
{

namespace text
{

class PROCESSINGFRAMEWORK_SHARED_EXPORT MultiObjectData : public InoutNode{
    Q_OBJECT
    Q_PROPERTY(int countArguments READ getCountArguments WRITE setCountArguments)
    Q_PROPERTY(int countObjects READ getCountObjects WRITE setCountObjects)
public:
    explicit MultiObjectData(QObject *parent);
    ~MultiObjectData();

    /*!Call from synchronizer
      * general rule frame = (0,length())
      * if frame = -1 it means to take next frame
      */
    virtual esp get(const base::timestamp frame);
    virtual void onConnected();
    int getCountArguments() {
        return countArguments;
    }
    int getCountObjects() {
        return countObjects;
    }
public slots:
    virtual void reset();
    void setCountArguments(int value) {
        if (value > 0) {
            countArguments = value;
        }
    }
    void setCountObjects(int value) {
        if (value > 0) {
            countObjects = value;
        }
    }
protected:
    virtual void play(esp data);
    virtual void clear();
    virtual bool save(QString path);
    virtual bool load(QString path);
    /*! Call from other Nodes
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    QString _separator;
    virtual void record(esp data);
    cv::Mat _mat, _mat2;
    int countArguments;
    int countObjects;
    int matSize;
    int constMatSize;
    int matStepSize;
    float emptyValue;
    types::Data rowMatToData(cv::Mat mat, int k);
    void dataToRowMat(types::Data data, cv::Mat *mat, int k);
    void pdataToRowMat(types::pData pdata, cv::Mat *mat, int k);
    void matPrint();
};
}
}
#endif // DATAIONODE1_H


