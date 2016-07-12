#ifndef SCRIPTWORKER_H
#define SCRIPTWORKER_H

#include "ScriptEditor_global.h"
#include <QObject>
#include <QtWidgets/QAction>
#include <QtWidgets/QListWidget>
#include <QMap>
class QScriptEngine;

class SCRIPTEDITORSHARED_EXPORT ScriptWorker : public QObject
{
    Q_OBJECT
public:
    explicit ScriptWorker(QObject *parent = 0);
    ~ScriptWorker();
    static QStringList getScripts(QString dir);
    QStringList getCompliterModel();
    QStringList getKeywords();
signals:
    void printSignal(QString outstr);
    void result(QString name,QVariant value);
public slots:
    /*!
     * \brief print
     * \param data
     */
    void print(QString data);
    /*! Добавление объекта из приложения в виде переменной скрипта
    \param obj - указатель на объект,
    \param name - псевдоним объекта*/
    void addObject(QObject* obj,QString name="");
    /*! Запуск скрипта по имени из списка доступных пользователю
      \param script - название скрипта
     */
    void runScript(QString script);
    /*! Установить таймер
      */
    void setTimer(int time);
    /*! Вызвать функцию реакции на таймер
      */
    void callTimer();
    //! Спать к-во миллисекунд
    void sleep(int msec);
    /*! Запомнить значение
      \param key название занчения
      \param value значение
      */
    void memorizeValue(QString key,double value);
    /*! Получить значение из память
        \param key название занчения
        \return значение из памяти. Ели значение нет возвращает ноль.
    */
    double getValue(QString key);
    /*!
     * \brief abortExecution отменить выполение операции
     */
    void abortExecution();
    /*! Очистить память */
    void cleanMemory();
    /*!
     * \brief replace Замена подстроки в строке
     * \param str Исходная строка
     * \param what Что
     * \param to на что заменяем
     * \return новая строка
     */
    QString replace(QString str,QString what,QString to);
    /*!
     * \brief executeApplication исполнить внешние приложение
     * \param appname название приложения
     * \param params параметры
     * \return вывод приложение в std::output
     */
    QString executeApplication(QString appname,QStringList arg);// arg1="",QString arg2="",QString arg3="");
    /*!
     * \brief getDateTime время текущее
     * \return возвращает текущее время в виде строки
     */
    QString getDateTime();
protected:
    //!Значения в памяти для работы между скриптами и для несколько раз запускаемых скриптов
    QMap<QString,double> _memvalues;
    //! Движок скриптовый
    QScriptEngine* _interpreter;
    //! Директория со скриптами
    QString _dir;
    //! Список ключевых функций
    QStringList _modellist;
    //! Список ключевых слов
    QStringList _keywords;
};

#endif // SCRIPTWORKER_H
