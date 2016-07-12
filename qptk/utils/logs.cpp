#include "logs.h"
#include <QApplication>
#include <QtDebug>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

using namespace utils;

Logger::Logger()
{
    _log = new QFile(QApplication::applicationName()+"log.txt");
}

Logger::~Logger()
{
    if(0!=_log){
        delete _log;
        _log = 0;
    }
}

void Logger::uninstall()
{
    writeLog("Ended: "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    if(0!=_instance)
    {
        delete _instance;
    }
}

Logger* Logger::_instance;
Logger* Logger::install()
{
    if(0==_instance)
    {
        _instance  = new Logger();
        qInstallMessageHandler(Logger::messageHandler);
        writeLog("Start: "+QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss"));
    }
    return _instance;
}
void Logger::writeInnerLog(QString msg)
{
    _log->open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(_log);
    ts << msg << endl;
    _log->close();
}

void Logger::writeLog(QString msg)
{
    Logger* logger = Logger::install();
    logger->writeInnerLog(msg);
}


QList<utils::Log> gWarning;
QList<utils::Log> gInfo;
QList<utils::Log> gCritical;
QList<utils::Log> gFatal;

QList<utils::Log> Logger::getMessages(QtMsgType type)
{
    switch (type) {
    case QtDebugMsg:
        return gInfo;
    case QtWarningMsg:
        return gWarning;
    case QtCriticalMsg:
        return gCritical;
    case QtFatalMsg:
        return gFatal;
    }
    return QList<utils::Log>();
}

Log::Log(QString fullmessage)
{
    time = QTime::currentTime();
    int begin = fullmessage.indexOf("$<<<",0);
    int end = fullmessage.indexOf(">>>$",0);
    if(-1!=begin&&-1!=end){
        location = fullmessage.mid(begin+4,end-begin);
        message = fullmessage.remove(begin,end+4);
    }else{
        message = fullmessage;
    }
}

void Logger::messageHandler(QtMsgType type,const QMessageLogContext& context, const QString& msg)
{
    QString txt;
    QTextStream cout(stdout,QIODevice::WriteOnly);
    switch (type) {
    case QtDebugMsg:
        txt = QString("Info: %1").arg(msg);
        gInfo.push_back(Log(msg));
        break;
    case QtWarningMsg:
        txt = QString("Warning: %1").arg(msg);
        gWarning.push_back(Log(msg));
        break;
    case QtCriticalMsg:
        txt = QString("Critical: %1").arg(msg);
        gCritical.push_back(Log(msg));
        break;
    case QtFatalMsg:
        txt = QString("Fatal: %1").arg(msg);
        gFatal.push_back(Log(msg));
        break;
    }
    cout << txt << endl;
 //   printf("")
    writeLog(txt);
    if(type==QtFatalMsg)
    {
        abort();
    }
}
