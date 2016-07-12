#ifndef %ClassName:u%_H
#define %ClassName:u%_H

#include <Processing/processnode.h>

using namespace processing;

class %ClassName% : public ProcessNode {
    Q_OBJECT
public:
    explicit %ClassName%(QObject *parent);
    ~%ClassName%();
protected:
    /*! Procesing in this fuction
      */
    virtual void receiveProcess(esp data);
};

#endif // %ClassName:u%_H

