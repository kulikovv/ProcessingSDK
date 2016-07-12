#ifndef EVENTSENDER_H
#define EVENTSENDER_H

#include <io/outputnode.h>

using namespace io;

class EventSender : public OutputNode {
    Q_OBJECT
    //TODO: Add your own propery to see them in GraphEdit
    //Q_PROPERTY( QString property READ property WRITE setProperty  )
public:
    explicit EventSender(QObject *parent);
    ~EventSender();
protected:
    /*! Call from other Nodes store your data here
      * \esp data container
      * \sa io::Node , EthoStudioSPointer
      */
    virtual void receiveProcess(esp data);
private:
    //Current data
};

#endif // EVENTSENDER_H

