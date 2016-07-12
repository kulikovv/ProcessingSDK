#include "parallel.h"
#include <opencv2/core/core.hpp>
#define __DO_PARALLEL__

/// use OpenMP for parallel execution of functions
//# include <omp.h>



using namespace workflow;

Parallel::Parallel(QObject *parent) :
    WorkflowNode(parent)
{
    _threadcount=0;
    _parnode=0;
    _datacont=0;
    _courent=0;
    setEnable(true);
}
void Parallel::receiveProcess(esp data)
{
  //  qDebug() << "added to " << sender()->objectName();
    QString name = sender()->objectName();
    if(!_counter.contains(name))
    {
        if(_courent<_threadcount)
        {
            _counter.insert(name,_courent++);
        }else{
            return;
        }
    }
    _datacont[_counter[name]] = data;
    _innerdata.insert(name,data.getFrameId());
    if(_threadcount==_innerdata.size())
    {
        doParallel();
    }

#ifdef __DO_PARALLEL__
        //#pragma omp parallel for shared(_nodes) private(i)
#endif // __DO_PARALLEL__

}
Parallel::~Parallel()
{
    if(0!=_parnode)
    {
        for(int i=0;i<_threadcount;i++)
        {
            delete _parnode[i];
        }
        delete []_parnode;
    }
    if(0!=_datacont)
    {
        delete []_datacont;
    }
}

void Parallel::setThreadCount(int value)
{
    if(value!=_threadcount)
    {

        if(!_nodes.isEmpty())
        {
            initNodes(value);
        }
    }
}
void Parallel::setNodesNames(QString value)
{
    WorkflowNode::setNodesNames(value);
   //Node* node = Node::classForName(name,this);
    if(!_nodes.isEmpty()&&0!=_threadcount)
    {
         initNodes(_threadcount);
    }
}

void Parallel::doParallel()
{
  //  qDebug() << "do parallel";
//    double start = cv::getTickCount();
    #pragma omp parallel for
    for(register int i=0;i<_threadcount;i++)
    {
        _parnode[i]->receive(_datacont[i]);
    }
   //  qDebug() << (cv::getTickCount()-start)/cv::getTickFrequency();
    _innerdata.clear();
}

void Parallel::initNodes(int value)
{
    _courent=0;
    _counter.clear();
    if(0!=_parnode)
    {
        for(int i=0;i<_threadcount;i++)
        {
            delete _parnode[i];
        }
        delete []_parnode;
    }
    _parnode = new Node*[value];
    _datacont = new esp[value];
    _threadcount = value;
    for(int i=0;i<_threadcount;i++)
    {
        _parnode[i] =  Node::classForName(_nodes.first()->objectName(),this);
        _parnode[i]->setEnable(true);
        connect(_parnode[i],SIGNAL(send(esp)),this,SLOT(innerReceive(esp)));
    }

}
void Parallel::innerReceive(esp data)
{
    for(int i=0;i<_threadcount;i++)
    {
        if(_parnode[i]==sender())
        {
            data.setTag(_counter.keys()[i]);
            emit send(data);
            break;
        }
    }
}
