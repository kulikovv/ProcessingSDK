#include "inoutnode.h"
#include <utils/logs.h>
#include <QFile>

using namespace io;

InoutNode::InoutNode(QObject *parent) :
    SourceNode(parent)
{
    _fps = 30;
    _recording = false;
    setEnable(false);
}

void InoutNode::setUrl(QString value)
{
    SourceNode::setUrl(value);
    clear();
    if(value.isEmpty())
    {
        setNodeType("Output");
        return;
    }
    try{
        if(QFile::exists(value))
        {
            if(load(value))
            {
                setEnable(true);
                _recording = false;
                setNodeType("Source");
            }else{
                WARNING("Error saving")
            }

        }else{
            INFO("File not exists")
            setEnable(true);
            _recording = true;
            setNodeType("Output");
        }

    }catch(std::exception ex){
         WARNING(QString(ex.what()))
    }catch(...){
        WARNING("Exception reading file")
        return;
    }
}

bool InoutNode::recording() const
{
    return _recording;
}

void InoutNode::play(esp data)
{
    get(data.getFrameId());
}

void InoutNode::receiveProcess(esp data)
{
    if(!_recording)
    {
        play(data);
    }else{
        record(data);
    }
}

void InoutNode::reset()
{
    setUrl("");
}


void InoutNode::setRecording(bool value)
{
    if(value)
    {
        setNodeType("Output");
    }
    if(_recording&!value){
        _recording = value;
        save(_url);
        emit ended(_url);
        setNodeType("Source");
    }
    _recording = value;

}
