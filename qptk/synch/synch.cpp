#include "synch.h"
#include <io/sourcenode.h>
#include <utils/logs.h>

using namespace synch;
using namespace base;
using namespace io;

Synch::Synch(QObject* obj):Graph(obj)
{
    _policy = Synch::None;
    _length = 0;
    setFps(25);
    setNodeType("Synch");
    setObjectName("Synch");
    timer.setTimerType(Qt::PreciseTimer);
}

QList<io::SourceNode*> Synch::getSources()
{
    //механизм остановки по определенному событию
    disconnect(this,SLOT(onStatus(QString,unsigned long,Node::Status)));
    //список источников
    QList<io::SourceNode*> sources;
    foreach(Node* node,_nodes.values())
    {
        if(!node)
        {
            continue;
        }
        //механизм остановки по определенному событию
        connect(node,SIGNAL(status(QString,unsigned long,Node::Status)),this,SLOT(onStatus(QString,unsigned long,Node::Status)));
        //если данный узел источник - то добавляем его
        io::SourceNode* source = qobject_cast<io::SourceNode*>(node);
        if(0!=source)
        {
            sources << source;
        }
    }
    return sources;
}

QIcon Synch::getIcon()
{
    return QIcon(":/sdk/synch/Synch");
}

QList<unsigned long> Synch::calcIntervals(unsigned long lenght)
{
    QList<unsigned long> times;

    foreach(SourceNode* node,getSources())
    {
        times << node->fps()*node->lenght()/lenght;
    }
    return times;
}

int Synch::add(Node* node,QString source)
{
    io::SourceNode* sourcenode = qobject_cast<io::SourceNode*>(node);
    if(0!=sourcenode)
    {
        connect(sourcenode,SIGNAL(lengthChanged(long)),this,SLOT(setLength(long)));
    }
    return Graph::add(node,source);
}

QList<unsigned long> Synch::calcSteps(int fps)
{
    QList<unsigned long> steps;
    foreach(SourceNode* node,getSources())
    {
        steps << node->fps()*node->lenght()/fps;
    }
    return steps;
}

double Synch::fps() const
{
    return _fps;
}

long Synch::length() const
{
    return _length;
}

void Synch::setFps(double fps)
{
    _fps = fps;
 //   timer.stop();
    timer.setInterval(1000/fps);

  //  timer.start();
    emit fpsChanged(_fps);
}

void Synch::setLength(long length)
{
    _length = length;
    INFO(QString("%1").arg(length));
    emit lengthChanged(_length);
}
void Synch::onStatus(QString status,unsigned long value,Node::Status stats)
{
     Q_UNUSED(value)
   // INFO("Synch onStatus")
     if("stop"==status&&base::Node::Ready==stats)
     {
         stop();
     }
}

SourceNode* Synch::findLongest()
{
    unsigned long max = 0;
    SourceNode* maxPtr = 0;
    foreach(SourceNode* node,getSources())
    {
        if(node->fps()*node->lenght()>max)
        {
            max = node->fps()*node->lenght();
            maxPtr = node;
        }
    }
    return maxPtr;
}

SourceNode* Synch::findSmallest()
{
    unsigned long min = 0xFFFFFFFF;
    SourceNode* minPtr = 0;
    foreach(SourceNode* node,getSources())
    {
        if(node->fps()*node->lenght()<min)
        {
            min = node->fps()*node->lenght();
            minPtr = node;
        }
    }
    return minPtr;
}

int Synch::averageFps()
{
    int mean=0;
    QList<SourceNode*> sources = getSources();
    foreach(SourceNode* node,sources)
    {
        mean+=node->fps();
    }
    if(sources.count()>0)
    {
        return mean/sources.count();
    }
    return mean;
}

unsigned long Synch::averageLength()
{
    unsigned long mean=0;
     QList<SourceNode*> sources = getSources();
    foreach(SourceNode* node,sources)
    {
        mean+=node->lenght();
    }
    if(sources.count()>0)
    {
        return mean/sources.count();
    }
    return mean;
}
