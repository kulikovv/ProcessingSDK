#include "streamoutput.h"
#include <QFile>
#include <types/data.h>
#include <QDateTime>

using namespace io::text;

StreamOutput::StreamOutput(QObject* parent):io::OutputNode(parent)
{
    _writemode = StreamOutput::NEW;
    _extension = "txt";
    _separator = "\t";
    _counter = 0;
    setEnable(true);
}

StreamOutput::~StreamOutput()
{
    _file.close();
}


StreamOutput::Mode StreamOutput::writeMode()
{
    return _writemode;
}

void StreamOutput::setWriteMode(StreamOutput::Mode mode)
{
    if(_writemode!=mode)
    {
        _writemode = mode;
        setUrl(_url);
    }
}

void StreamOutput::setUrl(QString url)
{
    OutputNode::setUrl(url);

    if(_url.isEmpty())
    {
        return;
    }

    //Open file and store data
    if(_file.isOpen())
    {
        _file.close();
    }

    QString newname;
    bool print_columns=false;
    switch(_writemode)
    {
        case StreamOutput::APPEND:
            if(!QFile::exists(_url))
            {
                print_columns=true;
            }
            _file.setFileName(_url);
            _file.open(QIODevice::Append | QIODevice::Text);
        break;
        case StreamOutput::REWRITE:
            _file.setFileName(_url);
            _file.open(QIODevice::WriteOnly | QIODevice::Text);
            print_columns=true;
        break;
        case StreamOutput::NEW:
            newname = QString("%1_%2.txt").arg(_url.split(".txt").first()).arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss"));
            _file.setFileName(newname); _file.open(QIODevice::WriteOnly | QIODevice::Text);
            print_columns=true;
        break;
        default:break;
    }
 /*   if (!_file.open(QIODevice::Append | QIODevice::Text))
             return;*/

    if(!_file.isOpen())
    {
        return;
    }

    _stream.setDevice(&_file);

    if(print_columns)
    {

        QStringList cols = _columns.split(";",QString::SkipEmptyParts);
      //  stream <<_columns<< "\n";
        foreach(QString col,cols)
        {
            _stream <<col<< _separator;
        }
        _stream << "\n";
    }


}


void StreamOutput::reset()
{
    _counter = 0;
}
void StreamOutput::receiveProcess(esp data)
{
    QSharedPointer<QString> string = data.getReadOnly<QString>();
    if(!string.isNull())
    {
      //  qDebug() << "cols" << *string;
        _columns = *string;
        emit send(esp::init(new QString(_columns),0,""));
        return;
    }

    QSharedPointer<types::Data> res = data.getReadOnly<types::Data>();
    try{

        if(!res.isNull()&&_file.isOpen())
        {

            for(int i=0; i<res->size();i++)
            {
                _stream <<res->at(i)<< _separator;
            }
            _stream << "\n";
            _stream.flush();
            emit send(esp::init<types::Data>(new types::Data( *res),_counter++ ));
        }
    }
    catch(std::exception ex){
    }
}

