#include "dataoutput.h"


#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <utils/logs.h>
#include <exception>

using namespace io::text;
using namespace types;

DataOutput::DataOutput(QObject *parent) :
    OutputNode(parent)
{
    _extension = "txt";
}
DataOutput::~DataOutput()
{
    stop();

}

void DataOutput::setUrl(QString value)
{
    _url = value;
}

void DataOutput::receiveProcess(esp data)
{

    QSharedPointer<Data> res = data.getReadOnly<Data>();
    try{
    if(res.isNull())
    {
        WARNING("No data recived!")
        return;
    }
    _data.push_back(*res);


    }catch(std::exception ex){

        CRITICAL(QString("Exeption recived %1").arg(ex.what()))
    }

    emit send(data);

}

void DataOutput::stop()
{
    if(_url.isEmpty())
    {
        WARNING("No url added")
        return;
    }
    //Open file and store data
    QFile file(_url);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
             return;

    QTextStream stream(&file);

    stream << _data;

    file.close();
    OutputNode::stop();
}

