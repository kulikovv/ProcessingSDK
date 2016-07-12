#ifndef NEGATIVE_H
#define NEGATIVE_H

#include <process/processnode.h>

namespace process{

class PROCESSINGFRAMEWORK_SHARED_EXPORT Negative : public ProcessNode {
    Q_OBJECT
public:
    explicit Negative(QObject *parent);
    ~Negative();
protected:
    /*! Procesing in this fuction
      */
    virtual void receiveProcess(esp data);
};
}
#endif // NEGATIVE_H


