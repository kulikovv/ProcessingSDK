#include "outputnode.h"
#include <utils/logs.h>

using namespace io;

OutputNode::OutputNode(QObject* obj):BaseIONode(obj)
{
    setNodeType("Output");
    setEnable(false);
    setBehaviour(DoNothing);
}
OutputNode::~OutputNode()
{

}

void OutputNode::start()
{

}

QIcon OutputNode::getIcon()
{
    return QIcon(":/sdk/io/OutputNode");
}

void OutputNode::stop()
{
 //   emit status("stop",0,Node::Ready);
 //   emit ended(_url);
}
void OutputNode::setEnable( bool value )
{
    if(value)
    {
        start();
    }else{
        stop();
    }
    Node::setEnable(value);
}
