#include "baseionode.h"

using namespace io;

BaseIONode::BaseIONode(QObject *obj):base::Node(obj)
{
}

QString BaseIONode::url() const
{
    return _url;
}
void BaseIONode::setUrl(QString value)
{
    _url = value;
}
QString BaseIONode::extension() const
{
    return _extension;
}

QIcon BaseIONode::getIcon()
{
    return QIcon(":/sdk/io/BaseIONode");
}

void BaseIONode::reset()
{
    _url = "";
}
