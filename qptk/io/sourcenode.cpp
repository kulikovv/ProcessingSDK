#include "sourcenode.h"
#include "utils/logs.h"

using namespace io;

SourceNode::SourceNode(QObject* obj):BaseIONode(obj)
{
    setNodeType("Source");
    _length = 0;
    _fps = 0;
    setEnable(true);
    setBehaviour(DoNothing);
}
SourceNode::~SourceNode()
{

}

QIcon SourceNode::getIcon()
{
    return QIcon(":/sdk/io/SourceNode");
}

long SourceNode::lenght()
{
    return _length;
}
double SourceNode::fps()
{
    return _fps;
}
void SourceNode::receiveProcess(esp data)
{
    get(data.getFrameId());
}

void SourceNode::setFps(double fps)
{
    _fps = fps;
    emit fpsChanged(_fps);
}

void SourceNode::setLength(unsigned long length)
{
    _length = length;
    emit lengthChanged(_length);
}

