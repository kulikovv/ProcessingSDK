#ifndef LOGS_H
#define LOGS_H

#include <QtDebug>
#include <QTime>
#include <sdk_global.h>

#define MYLOG << "$<<<" << __FILE__ << __LINE__ << ">>>$"
#define CRITICAL(MESSAGE) qCritical() MYLOG << MESSAGE;
#define IFCRITICAL(PREDICATE,MESSAGE) if(!PREDICATE){ CRITICAL(MESSAGE) }

#define INFO(MESSAGE) qDebug() MYLOG << objectName() << MESSAGE;
#define IFINFO(PREDICATE,MESSAGE) if(!PREDICATE){ INFO(MESSAGE) }

#define WARNING(MESSAGE) qWarning() MYLOG << MESSAGE;
#define IFWARNING(PREDICATE,MESSAGE) if(!PREDICATE){ WARNING(MESSAGE) }

#define FATAL(MESSAGE) qFatal("") MYLOG << MESSAGE;
#define IFFATAL(PREDICATE,MESSAGE) if(!PREDICATE){ FATAL(MESSAGE) }

class QFile;
namespace utils
{

struct Log{
    Log(){

    }
    Log(QString fullmessage);


    QTime time;
    QString message;
    QString location;
};

class PROCESSINGFRAMEWORK_SHARED_EXPORT Logger
{
private:
    Logger();
    ~Logger();
    static Logger* _instance;
    QFile* _log;
    static void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
    void writeInnerLog(QString msg);
public:
    static QList<utils::Log> getMessages(QtMsgType type);
    static void uninstall();
    static Logger* install();  
    static void writeLog(QString msg);
};

}

#endif // LOGS_H
