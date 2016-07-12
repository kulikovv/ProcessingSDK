#include "singleapplication.h"
#include <QSharedMemory>
#include <utils/logs.h>
using namespace utils;

SingleApplication::SingleApplication(QString appname,int &argc, char **argv) :
    QApplication(argc, argv, true)
{
    setApplicationName(appname);
    _singular = new QSharedMemory(appname, this);
}


SingleApplication::~SingleApplication()
{
    if(_singular->isAttached())
	{
		_singular->detach();
		delete _singular;
	}
}

bool SingleApplication::notify ( QObject * receiver, QEvent * e )
{
	try{
		return QApplication::notify(receiver,e);
	}catch(...){
		WARNING("Unknown exception occurs")
	}
    return true;
}


bool SingleApplication::lock()
{

    if(_singular->attach(QSharedMemory::ReadOnly)){
        _singular->detach();
        return false;
    }

    if(_singular->create(1))
        return true;

    return false;
}
