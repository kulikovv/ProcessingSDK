// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.
/*! \file node.h
    \brief Базовые классы для создания плагинов
 */
#ifndef PROCESSINGFRAMEWORK_NODE_H
#define PROCESSINGFRAMEWORK_NODE_H

#include <base/dyncreation.h>
#include <sdk_global.h>
#include <base/esp.h>
#include <QObject>
#include <QPair>
#include <QList>
#include <QMetaType>
#include <QElapsedTimer>
#include <QVariant>
#include <QIcon>
#include <QFuture>
#include <QFutureWatcher>

#define BENCHMARK benchmark(__FILE__+__LINE__)
//! Зарегистрировать динамический класс \sa Node::classForName
#define REGISTER_DYN_NODE(classname,filename) base::Node::registerClass(#classname,new base::Factory<classname>(filename));
#define UNREGISTER_DYN_NODE(classname) base::Node::unregisterClass(classname);
#define REGISTER_DYN_NODE2(desiredname,classname) base::Node::registerClass(desiredname,new base::Factory<classname>);
/*! \brief Основные базовые классы для ввода-вывода информации
 * Пространство имен содержащий основные элементы системы с которыми
  * взаимодействиют приложения контрагенты
  */
namespace base
{

/*! \brief Базовый класс отвечающий за работу элементов приложения
  * В данный класс встроены фунции для учета за работой одного из элемента системы
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT Node: public QObject
{
    Q_OBJECT
     Q_CLASSINFO("objectName",tr("The name you can refer using the node in the script, and in automatic UI setup."))
    /*! \property Node::thread
        \brief нить в которой исполняется узел
        Если значение нити отлично от нуля то узел выполняется в нити с указанным индексом.

        По умолчанию равно нулю.
      */
    Q_CLASSINFO("asynch",tr("Specify the thread in which we node processing function is executed."))
    Q_PROPERTY( bool asynch READ asynch WRITE setAsynch )
    /*!
        \property Node::enable

        \brief Node state (состояние узла)

        С помощью данного свойства можно включать/выключать узел

        Значение по умолчанию зависит от узла. В исходном наслединке от Node оно true

        \sa Node::Behaviour, Node::disabledBehaviour
    */
    Q_CLASSINFO("enable",tr("The state of the selected node. If enable if true the processing is done, otherwise it's reacts according to it disabledBehaviour."))
    Q_PROPERTY( bool enable READ enable WRITE setEnable )

    /*!
        \property Node::disabledBehaviour

        \brief поведение узла в отключенном состоянии

        В отключенном состоянии узел имеет два режима работы: пропускать пришедшие данные без
        обработки следующему узлу в графе (PassingNext) или блокировать данные и не пересылать их
        (DoNothing)

        Значение по умолчанию зависит от узла. В исходном наслединке от Node оно PassingNext

        \sa Node::Behaviour
    */
    Q_CLASSINFO("disabledBehaviour",tr("Specify the behaviour of the node in disable state. <b>DoNothing</b> means that the node will not do anything. <b>PassingNext</b> means that the frame recived by the node will be passed to his connections."))
    Q_PROPERTY( Behaviour disabledBehaviour READ behaviour WRITE setBehaviour )
    /*!
        \property Node::nodeType

        \brief Название типа узла. Сделано, чтобы можно было больше трех типов узлов делать

        \sa Node::getNodeType
    */
    Q_CLASSINFO("nodeType",tr("Specify the virtual subclass of the node. For example InputNode etc."))
    Q_PROPERTY( QString nodeType READ getNodeType)
    /*!
     *\property Node::nodeExecutionTime
     *\brief Время исполнения узла в миллисекундах
     */
     Q_CLASSINFO("nodeExecutionTime",tr("Time that is needed to execute the processing in the node."))
    Q_PROPERTY(double nodeExecutionTime READ nodeExecutionTime)
    /*!
     *\property Node::processed
     *\brief Количество обработанных пакетов данных
     */
     Q_CLASSINFO("processed",tr("The number of processed frames by the node."))
    Q_PROPERTY(unsigned long processed READ processed)
    /*!
     *\property Node::processed
     *\brief Количество пропущенных данных, при асинхронном выполнении
     */
    Q_CLASSINFO("skipped",tr("The number of skipped frames in ansync mode."))
    Q_PROPERTY(unsigned long skipped READ skipped)
    /*!
      Поедение узла в отключенном состоянии
      */
    Q_ENUMS(Behaviour)
    Q_CLASSINFO("globalExecutionTime",tr("Time from the begin of the loop to the end of the node."))
    Q_PROPERTY(double globalExecutionTime READ globalExecutionTime)
    Q_PROPERTY(QString library READ library)
public:
    /*!
      Сообщение отправляемое после работы узла
      */
    enum Status {
        Ok,//!< Узел правильно отработал
        Error,//!< Произошла ошибка
        Ready,//!< Узел готов принять новый фрэйм
        Percent,//!< Процент обработки
        Benchmark//!< Измерение времени работы программы
    };

    /*! Поведение узла в неактивном состоянии \sa enable
      */
    enum Behaviour {
        DoNothing, //!< При получении сигнала ничего не делает
        PassingNext //!< При получении сигнала передает сигнал дальше по цепочке
    };
    /*! Создает узел и устанавливает его значения по умолчанию
      */
    explicit Node(QObject *parent = 0);
    ~Node();
    /*! Запросить номер нити
      */
    bool asynch() const;

    /*! Узнать поведение узла
      * \sa Behaviour
      */
    Behaviour behaviour() const;

    virtual bool enable() const;
    /*! Переопределить функцию для того чтобы опрдлит как будет вести себя узел после подключения
      к потомку.
      */
    virtual void onConnected();
    /*!
     * \brief время исполнения узла. происходит замер перед началом и перед концом. в асинхронном режиме немножко по другому:
     * время окончания считается время инициализации future переменной.
     * \return время исполнения узла
     */
    double nodeExecutionTime();
    /*!
     * \brief processed Количество обработанных пакетов данных
     * \return Количество обработанных пакетов данных
     */
    unsigned long processed();
    /*!
     * \brief skipped Количество пропущенных данных, при асинхронном выполнении
     * \return Количество пропущенных данных, при асинхронном выполнении
     */
    unsigned long skipped();
    /*!
     * \brief globalExecutionTime Время, прошедшие с запроса кадра, до окончания работы узла
     * \return
     */
    virtual double globalExecutionTime();
    QString library();
signals:
    /*! Текущий статус узла для узлов с долгой обработкой
      * \param status название состояние например "загрузка"
      * \param percent процент до заверщшения
      */
    void status(QString status,unsigned long value,Node::Status stat);
    /*! Сигнал для передачи данных дальше по цепочке
      * \param data умный указатель на данные
      */
    void send(esp data);
public slots:
    /*!
     * \brief setAsynch настроить асинхронную работу узла
     * \param value значение
     */
    void setAsynch(bool value);
    /*! Возвращает иконку соответсвующую узлу
      \return Иконка, если такая есть
      */
    virtual QIcon getIcon();
    /*! Установить поведение узла в неактивном состоянии
      * \param value новое знаычение поведения
      * \sa Behaviour
      */
    void setBehaviour(Behaviour value);
    /*! Включить/выключить узел
      * \param value новое значение
      */
    virtual void setEnable( bool value );
    /*! Слот срабатывающий при получении сигнала от другого узла
      * если enable равно false, то поведение зависит от behaviour
      * \param data умный указатль на данные
      * \sa Behaviour, enable
      */
    void receive(esp data);
    /*! Реакция на пользовательскую комманду (например для сохраения данных)
      * \param key ключ
      * \param value значение передаваемое
      */
    virtual void userCommand(QVariant key,QVariant value);
    /*! \brief Обнуление сохраненных внутренних параметров узла
      Если узел использует какие то особые внутренние параметры для работы и при следущем
      запуске их необходимо обновить то следует перегрузить эту функцию.
      */
    virtual void reset();
    /*! Возвращает тип узла
      */
    QString getNodeType();
protected:
    /*! Переопределяемая функция обработки данных
      * т.к. скорость вызова виртуальной функции >> чем вызов виртуального СЛОТа то
      * мы ее выделили
      */
    virtual void receiveProcess(esp data)=0;

    /*! Отображение диалога с настройками
      * переопределяемый в наследниках
      * \sa config
      */
    virtual void configDialog();
    /*! Вывести в консоль замеры производительности
      */
    void printBeachmarks();
    /*! Задать тип узла
      */
    void setNodeType(QString type);
    /*!
     * \brief presend добавить данные в список на отправку
     * \param data данные
     */
    void presend(esp data);
public:
    /*! Механизм создания динамических классов по названию во время исполнения
      * \sa REGISTER_DYN_CLASS(classname)
      */
    static void registerClass(BaseFactory*  fact);
	static void unregisterClass(QString name);
    /*! Механизм создания динамических классов по названию во время исполнения
      * без использования динамического фрэмворка типов
      * \sa REGISTER_DYN_CLASS(classname)
      */
    static void registerClass(QString name,BaseFactory*  fact);
    //! Создать узел по названию класса
    /*! \param name название класса
      */
    static Node * classForName(QString name);
    /*! Создает узел с родителем
      * \param name название класса
      * \param parent родитель
      */
    static Node * classForName(QString name,QObject* parent);
    /*! Список названий классов зарегистрированных в системе
      */
    static QStringList getNodesNames();
    /*!
     * \brief setLibrary
     * \param libraryname
     */
    void setLibrary(QString libraryname);
protected:
    /*! \brief Позволяет замерять время исполнения участка кода для оптимизации узла
      * Для подстановки
      */
    unsigned long benchmark(QString name);

    unsigned long frame; //!< Номер текущего кадр
    void tic(unsigned long frameid); //!< Начинает последоввательность измерений
    double tok(unsigned long frameid);//!< Выдает время с начала измерений
    static void clearStartMap();//!< Очищает карту запусков
private slots:
    /*!
     * \brief onAsyncFinished слот срабатывает, когда заврешается исполнение асинхронного узла
     */
    void onAsyncFinished();
private:
    unsigned long _processed;//!< Количество обработанных пакетов
    unsigned long _skipped;//!< Количество пропущенных кадров в асинхронном режиме
    double _nodeExecutionTime;//!< Задержка на обработку в конкретном узле
    double _globalExecutionTime;//!< Время, прошедшие с запроса кадра, до окончания работы узла

    bool _thread; //!< Асинхронный режим
    bool _enable; //!< Состояние: включе/выключен узел
    Behaviour _disabledBehaviour; //!< Поведение в выключенном состоянии
    QList<QPair<QString,double> > _benchmarks; //!< Набор измерений для оптиизации работы узла
    QElapsedTimer _timer;//!< Используется для замера производительности
    QString _node_type;//!< Тип узла: наример Process, Visual и т.д.
    QFuture<void> _future;//!< Состояние процесса
    QFutureWatcher<void> _watcher;//!< Наблюдатель за состояние процесса обработки
    static QMap<unsigned long,QElapsedTimer> _startmap;//!< Карта, хранящая запущенные таймеры для глобальной оценки всремени работы узлов
    QList<esp> _tosend;//!< Данные для отправки по заврешению цикла
    QString _library;//!< Библиотека из которой загружен узел
};

}

Q_DECLARE_METATYPE(base::Node::Status)

#endif // PROCESSINGFRAMEWORK_NODE_H
