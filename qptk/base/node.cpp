#include "node.h"
#include <QIcon>
#include <QMetaProperty>
#include <QDebug>
#include <QElapsedTimer>
#include <utils/logs.h>
#include <QtConcurrent/QtConcurrent>

#define CLOCK_MEMORY 50 //Сколько кадров помним о запусках циклов

using namespace base;

typedef QMap<QString, BaseFactory*>     ClassNameFactoryMap;
static ClassNameFactoryMap factoryMap;
QMap<timestamp,QElapsedTimer> Node::_startmap;

Node::Node(QObject *parent) :
    QObject(parent)
{
    setNodeType("Node");   
    setBehaviour(PassingNext);
    _timer.start();
    _thread = false;
    _processed = 0;
    _skipped = 0;
    _nodeExecutionTime = 0;
    _globalExecutionTime = 0;
    _library = "";
    connect(&_watcher, SIGNAL(finished()), this, SLOT(onAsyncFinished()),Qt::DirectConnection);
}

QString Node::library()
{
    return _library;
}

 void Node::setLibrary(QString libraryname)
 {
     _library = libraryname;
 }

void Node::tic(timestamp frameid)
{
    QElapsedTimer timer;
    timer.start();
    _startmap.insert(frameid,timer);
   // qDebug() << frameid << "tic:" << ((double)timer.nsecsElapsed())/1000000.0f;;
    if(_startmap.size()>CLOCK_MEMORY)
    {
        //qDebug() << "remove" << _startmap.keys().first();
        _startmap.remove(_startmap.keys().first());
    }
}

double Node::tok(timestamp frameid)
{
    if(_startmap.contains(frameid))
    {


        return (double)_startmap[frameid].nsecsElapsed()/1000000.0f;// (QDateTime::currentMSecsSinceEpoch() - _startmap[frameid]);

    }
    return -1;
}

Node::~Node(){
    INFO(QString("%1 finished; processed: %2; skipped: %3; execTime: %4 timeLag: %5;").arg(this->objectName()).arg(processed()).arg(skipped()).arg(nodeExecutionTime()).arg(globalExecutionTime()))
}

/*=============== STATUS INFO ======================*/
timestamp Node::processed(){
    return _processed;
}

timestamp Node::skipped()
{
    return _skipped;
}

double Node::nodeExecutionTime()
{
    return _nodeExecutionTime;
}

double Node::globalExecutionTime()
{
    return _globalExecutionTime;
}
/*================ NODE BEHAVIOUR ===================*/
bool Node::asynch() const
{
    return _thread;
}
void Node::setAsynch(bool value)
{
    _thread = value;
}

Node::Behaviour Node::behaviour() const
{
    return _disabledBehaviour;
}

void Node::setBehaviour(Behaviour value)
{
    _disabledBehaviour = value;
}

bool Node::enable() const
{
    return _enable;
}


void Node::setEnable( bool value )
{
    _enable = value;

}
/*======== LOOK AND FEEL ==================*/
QIcon Node::getIcon()
{
    return QIcon();
}

/*========= CUSTOM COMMANDS =============*/
void Node::onConnected()
{
    return;
}

void Node::userCommand(QVariant /*key*/, QVariant /*value*/)
{
    //Ничего не делаем по умолчанию
}

void Node::reset()
{

}

void Node::configDialog()
{

}

/*====== LOGIC ==========================*/
void  Node::onAsyncFinished()
{
    benchmark("Finished");
    emit status(this->objectName(),frame,Ready);
    _processed++;
    _nodeExecutionTime = ((double)_timer.nsecsElapsed()/1000000.0f);
   // qDebug() << frame << "ext:"  << _nodeExecutionTime;
    double time = tok(frame);
    if(time>0)
    {
        // qDebug() << frame << "tok:"  << time;
        _globalExecutionTime = time;
    }

    //Выдираем по очереди сообщения
    while(_tosend.count()>0)
    {
        esp data = _tosend[0];
        _tosend.removeAt(0);
        emit send(data);
    }

}

void Node::presend(esp data)
{
    _tosend.push_back(data);
}

void Node::receive(esp data)
{
    if(_future.isRunning())
    {
        _skipped++;
        return;
    }
    _benchmarks.clear();
    _timer.restart();
    frame = data.getFrameId();

    if(!_enable)
    {

        switch(_disabledBehaviour)
        {
            case DoNothing: break;
            case PassingNext:
            default: emit send(data); break;
        }
    }else{
        try{
            if(_thread)
            {
                _future = QtConcurrent::run(this,&Node::receiveProcess,data);
                _watcher.setFuture(_future);
            }else{
                receiveProcess(data);
            }
        }catch(std::exception& ex)
        {

           emit status(QString::fromStdString(ex.what()),0,Error);
        }catch(...){
           emit status("Unknown error",0,Error);
        }
    }
    if(!_thread)
    {
        onAsyncFinished();
    }

}


/* ===================== benchmark mechanism =========================*/
timestamp Node::benchmark(QString name)
{
    double exectime = ((double)_timer.nsecsElapsed()/1000000.0f);
    _benchmarks.push_back(QPair<QString,double>(name,exectime) );
    return exectime;
}

void Node::printBeachmarks()
{

    if(_benchmarks.count()<1)
        return;
    qDebug() <<"ID\tName\tTime(ms)\tDt(ms)";
    int i=0;
    typedef QPair<QString,double> btype;
    timestamp last=_benchmarks.first().second;
    foreach(btype mark,_benchmarks)
    {
        qDebug() << QString("%1\t%2\t%3\t%4").arg(i++).arg(mark.first).arg(mark.second).arg(mark.second-last);
        last = mark.second;
    }

}
/* ===================Dynamic node construction========================= */

void Node::registerClass(BaseFactory*  fact)
{
	
    QObject* tmp =fact->create(0);
    if(tmp)
    {      
        factoryMap.insert(tmp->metaObject()->className(),fact);
        delete tmp;
    }
}
void Node::unregisterClass(QString name)
{
    qDebug() << factoryMap;
	BaseFactory* fact = factoryMap.value(name,0);
	if(fact){
		delete fact;
		fact=0;
	}
	factoryMap.remove(name);
}

void Node::registerClass(QString name,BaseFactory*  fact)
{
     factoryMap.insert(name,fact);
}

Node * Node::classForName(QString name)
{
    BaseFactory* bf = factoryMap.value(name,0);
    if(bf)
    {
        return qobject_cast<Node*>(bf->create(0));
    }else{
        return 0;
    }
    return 0;
}

Node * Node::classForName(QString name,QObject* parent)
{
    BaseFactory* bf = factoryMap.value(name,0);
    if(bf)
    {
        return qobject_cast<Node*>(bf->create(parent));
    }else{
        return 0;
    }
    return 0;
}
void Node::clearStartMap()
{
    _startmap.clear();
}

QStringList Node::getNodesNames()
{
    return factoryMap.keys();
}

/*=============================== Node type section======================== */

void Node::setNodeType(QString type)
{
    _node_type = type;
}

QString Node::getNodeType()
{
    return _node_type;
}
