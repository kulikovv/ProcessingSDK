#ifndef %ClassName:u%_H
#define %ClassName:u%_H

#include <IO/sourcenode.h>

using namespace io;

class %ClassName% : public %Datatype%{
    Q_OBJECT
public:
    explicit %ClassName%(QObject *parent);
    ~%ClassName%();
    /*!Call from synchronizer 
      * general rule frame = (0,length())
      * if frame = -1 it means to take next frame
      */
    virtual esp get(const timestamp frame);
protected:
    /*! Call from other Nodes
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
};

#endif // %ClassName:u%_H

