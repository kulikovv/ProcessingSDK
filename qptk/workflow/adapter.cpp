#include "adapter.h"
#include <QDebug>
using namespace workflow;
Adapter::Adapter(QObject *parent) :
    WorkflowNode(parent)
{
    _selected = 1000;
}

void Adapter::receiveProcess(esp data)
{
    qDebug()<< "Adapter::receiveProcess" << _selected  ;
    if(_selected>=0&&_selected<_nodes.count()){
        _nodes[_selected]->setEnable(true);
        _nodes[_selected]->receive(data);
        _mem = data;
    }
}

int Adapter::selected() const
{
    return _selected;
}

void Adapter::setSelected(int x)
{
   // if(x>=0&&x<_nodes.count())
  //  {
        _selected = x;
        receiveProcess(_mem);
 //   }
}

void Adapter::addNode(base::Node* node)
{
	WorkflowNode::addNode(node);
	connect(node,SIGNAL(send(esp)),this,SIGNAL(send(esp)));
}
void Adapter::userCommand(QVariant key,QVariant value)
{
    if("setSelect"==key.toString())
    {
        qDebug()<< "Selected" << value.toInt();
        setSelected(value.toInt());
    }
}
