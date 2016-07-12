#ifndef %ClassName:u%_H
#define %ClassName:u%_H

#include <IO/outputnode.h>

using namespace io;

class %ClassName% : public OutputNode {
    Q_OBJECT
    //TODO: Add your own propery to see them in GraphEdit
    //Q_PROPERTY( QString property READ property WRITE setProperty  )
public:
    explicit %ClassName%(QObject *parent);
    ~%ClassName%();
protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
private:
    //Current data
};

#endif // %ClassName:u%_H

