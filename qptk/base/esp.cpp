#include "esp.h"
#include <map>
#include <base/dynconvert.h>

using namespace base;
typedef map<string, ConvertFactory*> ClassNameFactoryMap;
static ClassNameFactoryMap factoryMap;

Esp::Container::Container(void* ptr,void (*releaseFunc)(void*))
{
    _ptr = ptr;
    _releaseFunc = releaseFunc;

}

Esp::Container::~Container()
{
    _releaseFunc(_ptr);
    _ptr=0;
}

Esp::Esp(QString type,Container* data,timestamp frame,QString tag)
{
   _data = QSharedPointer<Container>(data);
   _type = type;
   _tag = tag;
   _timestamp = frame;
}

Esp::Esp()
{
    _timestamp = 0;
    _type = "";
    _tag = "";
}
/*
Esp::Esp(Esp &rN)
{
    _data =rN._data;
    _type = rN._type;
    _tag = rN._tag;
    _timestamp = rN._timestamp;
}*/

timestamp Esp::getFrameId() const
{
    return _timestamp;
}
void Esp::registerConvertor(ConvertFactory* cv)
{
    factoryMap[cv->name()]=cv;
}

void* Esp::convert(string type)
{
    ConvertFactory* fc = factoryMap[_type.toStdString()+type];
    if(0!=fc)
    {
        return fc->convert(_data.data());
    }
    return 0;
}
bool Esp::operator==(const QString &other) const {
    int res = QString::compare(_tag,other,Qt::CaseInsensitive);
    if(0==res){
        return true;
    }else{
        return false;
    }
}

void Esp::setTag(QString tag)
{
    _tag = tag;
}

bool Esp::operator!=(const QString &other) const {
    int res = QString::compare(_tag,other,Qt::CaseInsensitive);
    if(0==res){
        return false;
    }else{
        return true;
    }
}
