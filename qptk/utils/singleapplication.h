#ifndef SINGLEAPPLICATION_H
#define SINGLEAPPLICATION_H

#include <QApplication>
#include <sdk_global.h>

class QSharedMemory;

namespace utils
{
/*!
  \brief Класс для приложений которые должны иметь один экземпляр в системе
  Реализует в соотвествии с Qt Solution одиночное приложение. Дополнительно можно проверять
  было ли приложение корректно завершино, или упало в ошибку.
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT SingleApplication : public QApplication
{
    Q_OBJECT
public:
    explicit SingleApplication(QString appname,int &argc, char **argv);
    ~SingleApplication();
    bool lock();
	virtual bool notify ( QObject * receiver, QEvent * e );
private:
    QSharedMemory *_singular;

};
}

#endif // SINGLEAPPLICATION_H
