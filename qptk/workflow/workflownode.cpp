#include "workflownode.h"
#include  <QStringList>
#include <QDebug>
using namespace workflow;

WorkflowNode::WorkflowNode(QObject *parent) :
    base::Node(parent)
{
    setNodeType("Workflow");
}
void WorkflowNode::addNode(base::Node* node)
{
    foreach(base::Node* n,_nodes)
    {
        if(n->objectName()==node->objectName())
        {
            return;
        }
    }

    _nodes.push_back(node);
    connect(node,SIGNAL(destroyed(QObject*)),this,SLOT(removeNode(QObject*)));
}

void WorkflowNode::removeNode(QObject* node)
{
    base::Node* node_real = qobject_cast<base::Node*>(node);
    if(0!=node_real)
    {
        _nodes.removeOne(node_real);
    }
}
QString WorkflowNode::nodesNames() const
{
    QString allfix;
    foreach(Node* node,_nodes)
    {
        allfix += node->objectName()+";";
    }
    return allfix;
}

void WorkflowNode::setNodesNames(QString value)
{
    QStringList data = value.split(";",QString::SkipEmptyParts);
    foreach(Node* node,_nodes)
    {
        removeNode(node);
    }
    _nodes.clear();

    foreach(QString name,data)
    {
        Node* node = Node::classForName(name,this);
        if(0!=node)
        {

            node->setObjectName(name);
            this->addNode(node);
        }else{
            qCritical() << "Can't load" << name;
        }
    }
}
QList<base::Node*> WorkflowNode::getNodes() const
{
    return _nodes;
}
