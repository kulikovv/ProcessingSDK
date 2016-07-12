#include "eventsender.h"
#include <types/event.h>
EventSender::EventSender(QObject *parent) :
    OutputNode(parent)
{
    //TODO: Initiation source
}

EventSender::~EventSender()
{
    //TODO: Destruction
}

//This function is called if this node is enable
void EventSender::receiveProcess(esp data)
{
    //TODO: Conversion to your type
    QSharedPointer<QString> x = data.getReadOnly<QString>();
    if(!x.isNull())
    {
        //TODO: storing data code here
    }
}
