#ifndef BINARYNODE_H
#define BINARYNODE_H

#include <process/processnode.h>
#include <opencv2/core/core.hpp>

namespace process{
class PROCESSINGFRAMEWORK_SHARED_EXPORT BinaryNode : public ProcessNode {
    Q_OBJECT
    Q_PROPERTY(QString maskUrl READ maskUrl WRITE setMaskUrl)
    Q_PROPERTY( bool inverted READ isInverted WRITE setInverted USER true)
    Q_PROPERTY(int threshold READ threshold WRITE setThreshold USER true)
    Q_PROPERTY(bool adaptive MEMBER _adaptive USER true)
    Q_PROPERTY(int adaptiveSize MEMBER _adaptiveSize USER true)
public:
    explicit BinaryNode(QObject *parent);
    QString maskUrl();
    bool isInverted();
    int threshold();
public slots:
    virtual QIcon getIcon();
    void setThreshold(int value);
    void setInverted(bool value);
    void setMaskUrl(QString str);
protected:
    /*! Procesing in this fuction
      */
    virtual void receiveProcess(esp data);
    cv::Mat grey;
private:
    QString _maskUrl;
    
    bool _inverted;
    int _threshold;
    bool _adaptive;
    int _adaptiveSize;
protected:
	cv::Mat _mask;
};
}
#endif // BINARYNODE_H


