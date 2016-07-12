#ifndef %ClassName:u%_H
#define %ClassName:u%_H

#include <Visual/visualnode.h>

using namespace visual;

class %ClassName% : public VisualNode {
    Q_OBJECT
public:
    explicit %ClassName%(QObject *parent);
    ~%ClassName%();
protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);

};

#endif // %ClassName:u%_H

