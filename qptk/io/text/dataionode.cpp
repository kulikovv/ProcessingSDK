#include "dataionode.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFile>
#include <utils/logs.h>
#include <exception>
#include <QFileDialog>


using namespace io;
using namespace io::text;
using namespace types;

DataIONode::DataIONode(QObject *parent) :
    InoutNode(parent)
{
    _extension = "txt";
    _separator = "\t";
    setEnable(true);
    _interpolation = true;
 //   _data.resize(1);
}

DataIONode::~DataIONode()
{
    clear();
}
void DataIONode::play(esp data)
{
  /*  QSharedPointer<QString> header = data.getReadOnly<QString>();
    if(!header.isNull())
    {
        setColumns(*header.data());
    }*/
    get(data.getFrameId());
}

void DataIONode::reset()
{
    INFO("Reset")
    _extension = "txt";
    _separator = "\t";
    setEnable(true);
    _interpolation = false;
    clear();
    InoutNode::reset();
}

esp DataIONode::get(const unsigned long frame)
{   
    //INFO("Get")
    esp pointer;
    try{
        if(frame<_data.size())
        {
            if(!_data[frame].isempty())
            {
               // std::copy(_data[frame]-)

                pointer = esp::init(new Data(_data[frame]),frame);
                emit send(pointer);
            }
        }
    }catch(...)
    {
        qDebug() << "Exception";
    }
  /*  QStringList list;
    for(int i=0;i<pointer.getReadOnly<Data>()->size();i++)
    {
        list.push_back(QVariant((*pointer.getReadOnly<Data>())[i]).toString());
    }
    qDebug() << list;*/
    return pointer;
}
void DataIONode::record(esp data)
{
  /*  QSharedPointer<QString> header = data.getReadOnly<QString>();
    if(!header.isNull())
    {
        setColumns(*header.data());
        return;
    }*/

    QSharedPointer<Data> res = data.getReadOnly<Data>();
    try{
      //Процедура интерполяции
        if(!res.isNull()){
            if(!res->isempty())
            {
                try{
                    _data.push_back(*res);
                }catch(types::unexpected_header err){
                    _data.clear();
                    _data.push_back(*res);
                }

                SEND(types::Data,new types::Data( _data[frame]));
            }else{
                qDebug() << "Empty data recive" << frame;
            }
        }
        /*if(_interpolation)
        {
            int first_size = _data.size();
            if((data.getFrameId()+1)>first_size)
            {
             //   _data.resize(data.getFrameId()+1);
            }
            for(unsigned int i=first_size;i<_data.size();i++)
            {
                if(!res.isNull())
                {
                    _data[i] = *res;
                }else if(i>0)
                {
                    _data[i] = _data[i-1];
                    emit send(esp::init<types::Data>(new types::Data(_data[i]),i));
                }else{
                   // _data[i] = new types::Data( header);

                }
            }
        }*/

     //   if(!res.isNull())
        {
   /*         if(_interpolation)
            {
                if(0!=_data[data.getFrameId()])
                {
                    delete _data[data.getFrameId()];
                    _data[data.getFrameId()]= 0;

                }

                _data[data.getFrameId()] = new types::Data( *res);
                emit send(data);
            }else{
                if(res->size()>0)
                {
                    if(data.getFrameId()<_data.size()){
                        delete _data[data.getFrameId()];
                        _data[data.getFrameId()]=(new types::Data( *res));
                    }else{
                        _data.push_back(new types::Data( *res));
                    }

                    SEND(types::Data,new types::Data( *res))
                 //   emit send(esp::init<types::Data>(new types::Data( *res),_data.size()-1 ));
                }
            }*/
        }

    }
    catch(std::runtime_error re){
        CRITICAL(QString("Runtime error %1").arg(re.what()))
    }catch(std::exception ex){
        CRITICAL(QString("Exeption recived %1").arg(ex.what()))
    }catch(...){
        CRITICAL(QString("Unknown error"))
    }
}

bool DataIONode::save(QString path)
{
   // INFO(QString("Saving to %1 %2 rows").arg(path).arg(_data.size()))
    //Open file and store data
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QString fname = QFileDialog::getSaveFileName(0,tr("Save track"),QString(),"*.txt");
        if(fname.isEmpty()){
            return false;
        }
        if(!fname.contains(".txt")){
            fname += ".txt";
        }
        file.setFileName(fname);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
            return false;
        }
    }


    QTextStream stream(&file);

    stream << _data;

    file.close();
    return true;
}


bool DataIONode::load(QString path)
{

    //Open file and copy data
    QFile file(path);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
             return false;
    clear();

    QTextStream stream(&file);

    stream >> _data;
//    qDebug() << _data.header();
 /*   QStringList cols;
    QString line;
    line=stream.readLine();//Read header

    QStringList header = line.split("\t", QString::SkipEmptyParts);
    qDebug()<<header;

    _length = 0;
    line=stream.readLine();
    while(!line.isEmpty())
    {
      cols = line.split(_separator, QString::SkipEmptyParts);
      Data x(header,_length);
      for(int i=0;i<cols.size();i++)
      {
          x[i]=cols[i].toFloat();
      }
       _data.push_back(x);
      _length++;
      line=stream.readLine();
    }*/
    file.close();

	//INFO("Send full data")
    onConnected();
   // INFO(QString("Reading end %1").arg(_length))
    return true;
}
bool DataIONode::interpolation()
{
    return _interpolation;
}

void DataIONode::setInterpolation(bool value)
{
    _interpolation = value;
}

void DataIONode::onConnected()
{

    if(_data.size()>0)
    {        
        pData* copy = new pData(_data);
       /* for(unsigned int i=0;i<_data.size();i++)
        {
            copy->push_back(Data(*_data[i]));
        }*/
		//INFO("Send full data")
        emit send(esp::init(copy,0,"Full"));

    }
}

void DataIONode::clear()
{
 //   INFO("Clean started")

    _data.clear();

    emit send(esp::init(new pData(),0,"Full"));

}
const types::pData& DataIONode::getData()
{
    return _data;
}

/*
QString DataIONode::columns()
{
    return _header.join(";");
}

void DataIONode::setColumns(QString value)
{
    if(_data.empty())//Если нет данных, то можно менять
    {
        _header = value.split(";", QString::SkipEmptyParts);
        emit send(esp::init(new QString(columns()),0,""));
        try{

        }catch(std::exception ex){
            qDebug() << ex.what();
        }
    }
}
*/
