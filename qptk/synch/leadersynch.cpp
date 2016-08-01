#include "leadersynch.h"
#include <io/sourcenode.h>
#include <utils/logs.h>
#include <QtConcurrent/QtConcurrent>
//#include <omp.h>

using namespace io;
using namespace synch;

LeaderSynch::LeaderSynch(QObject* obj):Synch(obj)
{
    _current=0;//Текущий номер кадра
    _loop = false;//По умолчанию ничто не крутиться в цикле
    this->connect(&timer,SIGNAL(timeout()),this,SLOT(run()));
}
void LeaderSynch::pause()
{
    if(timer.isActive()){
        timer.stop();
    }else{
        timer.start();
    }

}
void LeaderSynch::seek(unsigned long pos)
{
    if(pos>=0&&pos<this->_length)
    {
        foreach(SourceNode* node,_sources)
        {
            node->receive(esp::init<int>(new int,pos));
        }
        _current = pos;
    }
    if(0==_length&&0==pos)
    {
        _current = 0;
    }

}
unsigned long LeaderSynch::current()
{
    return _current;
}

bool LeaderSynch::loop() const
{
    return _loop;
}

void LeaderSynch::setLoop(bool value)
{
    _loop = value;
}
void LeaderSynch::step()
{
    if(_sources.isEmpty()){
         _sources = getSources();
    }
    run();
}

void LeaderSynch::run()
{
    tic(_current);
    emit currentFrame(_current);
    if(0!=this->_length)
    {
        if(this->_length <= _current)
        {
            if(_loop){
                _current = 0;
            }else{
                emit finished();
                stop();
                return;
            }
        }
    }

   /* if(_current>0)
    {
    //    qDebug() << tok(_current)-tok(_current-1);
    }*/
   // int i;
 //   #pragma omp parallel for
    for(int i=0;i<_sources.count();i++)
    {
    //    qDebug() << _current << i;
        _sources[i]->receive(esp::init<int>(new int,_current));
    }
    _current++;
  //  INFO("Current frame:"+QString("%1").arg(_current))


}

void LeaderSynch::start()
{
    this->clearStartMap();
     _sources = getSources();
  /*  if(0==_sources.count())
    {
        return;
    }
    QList<base::timestamp> intervals;
    SourceNode* node=0;


    switch(_policy)
    {
    case Synch::Longest:
        node = this->findLongest();
        break;
    case Synch::Smallest:
        node = this->findLongest();
        break;
    case Synch::Medium:

        break;
    case Synch::LeaderSource:
        node = _sources.at(0);
        break;
    case None:
        node = _sources.at(0);
        break;
    }
    if(!node)
    {
        CRITICAL("Source list is empty")
        return;
    }
    if(0!=node->fps())
    {
                timer.setInterval(1000/node->fps());
                setFps(node->fps());
    }
    setLength(node->lenght());
    connect(node,SIGNAL(fpsChanged(double)),this,SLOT(onFpsChange(double)));
    connect(node,SIGNAL(ended(QString)),this,SLOT(stop()));*/
    //_current = 0;
    if(!timer.isActive()){
         timer.start();
     }

}

void LeaderSynch::onFpsChange(double fps)
{
    setFps(fps);

}
bool LeaderSynch::isRunning()
{
    return timer.isActive();
}

void LeaderSynch::stop()
{
    if(isRunning()){
        timer.stop();
        emit stopped();
        _current=0;
    }
}
