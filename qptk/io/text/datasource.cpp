#include "datasource.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFile>

using namespace io;
using namespace io::text;
using namespace types;

DataSource::DataSource(QObject * /*parent*/)
{
    _extension = "txt";
    _fps = 30;
}

esp DataSource::get(const unsigned long frame)
{
    esp pointer;
    try{
        if(frame<_data.size()&&((uint)-1)!=frame)
        {
            pointer = esp::init(new Data(_data[frame]),frame);
            emit send(pointer);
        }
    }catch(...)
    {}
    return pointer;
}

void DataSource::receiveProcess(esp data)
{
    get(data.getFrameId());
}

void DataSource::onConnected()
{
    if(_data.size()>0)
    {
        emit send(esp::init(new pData(_data),0,"Full"));
    }
}

void DataSource::setUrl(QString value)
{
    _url = value;
    //Open file and copy data
    QFile file(value);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return;
    QTextStream stream(&file);

    stream >> _data;


    _length = _data.size();

    file.close();
    onConnected();
}
