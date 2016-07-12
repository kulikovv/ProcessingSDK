#include "scriptworker.h"
#include <QDir>
#include <QMessageBox>
#include <QMetaMethod>
#include <QList>
#include <QMetaProperty>
#include <QtScript>
#include <QMetaClassInfo>
#include <QProcess>

Q_DECLARE_METATYPE(QList<double> )
Q_DECLARE_METATYPE(QList<QList<double> >)
ScriptWorker::ScriptWorker(QObject *parent) :
    QObject(parent)
{
    _interpreter = new QScriptEngine();
    qScriptRegisterSequenceMetaType<QStringList>(_interpreter);
    qScriptRegisterSequenceMetaType<QList<double> >(_interpreter);
    qScriptRegisterSequenceMetaType<QList<QList<double> > >(_interpreter);
}
ScriptWorker::~ScriptWorker()
{
    if(_interpreter){
        delete _interpreter;
        _interpreter = 0;
    }
}

QStringList ScriptWorker::getScripts(QString scriptdir)
{
    QDir dir(scriptdir);
    QStringList list;
    list << "*.js";
    return dir.entryList(list,QDir::Files);
}

QStringList ScriptWorker::getCompliterModel()
{
    return _modellist;
}

QStringList ScriptWorker::getKeywords()
{
    return _keywords;
}
void ScriptWorker::abortExecution()
{
     if(_interpreter)
     {
         QScriptValue objectValue;
         _interpreter->abortEvaluation(objectValue);
         emit result("Abort",objectValue.toVariant());
     }
}
void ScriptWorker::addObject(QObject* obj,QString name)
{
    QScriptValue objectValue = _interpreter->newQObject(obj);
    if(name.isEmpty())
    {
        if(!obj->objectName().isEmpty())
        {
            name = obj->objectName();
        }else{
            name = obj->metaObject()->className();
        }
    }
    _interpreter->globalObject().setProperty(name, objectValue);
    _keywords << name;

    const QMetaObject* metaObject = obj->metaObject();
    //Добавление в подсказчик методов


    //Добавление в подсказчик свойств
    do
    {
        if("QObject"==QString(metaObject->className()))
        {
            continue;
        }
        for(int i=metaObject->methodOffset();i<metaObject->methodCount();i++)
        {
            QMetaMethod mm = obj->metaObject()->method(i);
            if(QMetaMethod::Slot == mm.methodType() && QMetaMethod::Public==mm.access())
            {
                const char* sig = mm.methodSignature();//mm.signature();
                QString syntex = QString("%1.%2").arg(name).arg(QString(sig));
                if(!_modellist.contains(syntex,Qt::CaseInsensitive))
                {
                    _modellist << syntex;
                }
            }
        }

        int count = metaObject->propertyCount();
        for (int i=0; i<count; ++i)
        {
            QMetaProperty property = metaObject->property(i);
                        //if( property.isUser() ) // Hide Qt specific properties
            {
                QString syntex = QString("%1.%2").arg(name).arg(QString(property.name()));
                if(!_modellist.contains(syntex,Qt::CaseInsensitive))
                {
                    _modellist << syntex;
                }
            }
        }
    }
    while ((metaObject = metaObject->superClass())!=0);


}
void ScriptWorker::print(QString data)
{
    emit printSignal(data);
}

QString ScriptWorker::executeApplication(QString appname, QStringList arg)
{
    QStringList args=arg;
/*    if(!arg1.isEmpty())
        args.push_back(arg1);
    if(!arg2.isEmpty())
        args.push_back(arg2);
    if(!arg3.isEmpty())
        args.push_back(arg3);*/

    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    process.start(appname,args);
    if (!process.waitForStarted())
        return "Not started";

    if(!process.waitForFinished())
    {
        process.close();
        return "Not ended";
    }

    return QString(process.readAll());
}

QString ScriptWorker::getDateTime()
{
    return QDateTime::currentDateTime().toString();
}

QString ScriptWorker::replace(QString str,QString before,QString after)
{
   return str.replace(before,after);
}

void ScriptWorker::setTimer(int time)
{
    QTimer::singleShot(time,this,SLOT(callTimer()));
}

void ScriptWorker::callTimer()
{

    QScriptValue global = _interpreter->globalObject();
    QScriptValue timer = global.property("onTimer");
    QScriptValue value = timer.call(QScriptValue());
    if(value.isError())
    {
        QMessageBox::critical(0,tr("Error"),value.toString());
    }
    if(!value.isError())
    {
        emit result("Main",value.toVariant());
    }
}

void ScriptWorker::runScript(QString script)
{
    QScriptValue value = _interpreter->evaluate(script);
    if (_interpreter->hasUncaughtException()) {
                emit result("Exception",value.toVariant());
                QMessageBox messageBox;
                messageBox.setIcon(QMessageBox::Warning);
                messageBox.setWindowTitle(tr("Script Editor"));
                messageBox.setText(tr("An error occurred while executing the script."));
                messageBox.setInformativeText(
                        tr("%1.").arg(_interpreter->uncaughtException()
                                      .toString()));
                messageBox.setDetailedText(
                        _interpreter->uncaughtExceptionBacktrace().join("\n"));
                messageBox.exec();
                return;
    }

    if(!value.isError())
    {
        emit result("Success",value.toVariant());
    }else{
        emit result("Error",value.toVariant());
    }

}

void ScriptWorker::sleep(int msec)
{
    QEventLoop loop; QTimer::singleShot(msec, &loop, SLOT(quit())); loop.exec();
}

void ScriptWorker::memorizeValue(QString key,double value)
{
    _memvalues.insert(key,value);
}

double ScriptWorker::getValue(QString key)
{
    return _memvalues.value(key,0);
}
void ScriptWorker::cleanMemory()
{
    _memvalues.clear();
}

