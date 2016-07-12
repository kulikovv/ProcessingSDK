#ifndef PROCESSINGFRAMEWORK_GRAPH_H
#define PROCESSINGFRAMEWORK_GRAPH_H

#include <base/node.h>
#include <QObject>
#include <QPair>
#include <QSharedPointer>
/*! \brief Базовые классы SDK включающие узлы и данных
  *
  */
namespace base
{
/*! \brief Граф состоящий из узлов, задает логику работы конвеера
  *
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT Graph : public Node
{
    Q_OBJECT
    /*! Провека является ли граф циклическим
      если данная опция подключена, то link не будет возвращать ощибку если найден цикл
     */
    Q_CLASSINFO("allowCyclic",tr("If this property value is true than you can make a cicle in graph, but you are responsable of possible deadlocks. By default should be false"))
    Q_PROPERTY( bool allowCyclic READ allowCyclic WRITE setAllowCyclic )
    Q_CLASSINFO("estimatedFps",tr("Estimed fps of the graph"))
    Q_PROPERTY(double estimatedFps READ estimatedFps)
public:
    typedef QPair<Node*,Node*> Connection;

    /*! Создает новый граф
      */
    explicit Graph(QObject *parent = 0);
    ~Graph();
    /*! Связать два узла сигнал send(esp data) и receive(esp data),
      * а также связывает configChanged и config, чтобы дать возможность узлам
      * передавать свои настройки и состаяние другим
      * если образуется циклическая зависимость возвращает false

      * \param in узел от которого идет поток информации
      * \param out узел в который идет поток информации
      * \return обычно true, false если один из указателей пуст или образуцется цилическая зависимость в графе
      * \sa send, receive
      */
    bool link(Node *in,Node *out);

    /*! Добавляет узел в граф
      */
    virtual int add(Node* node,QString source="");

    /*!
     * \brief addGraph вставляет подграф в граф
     * \param graph вставляемый граф
     * \return true, если успех, false - иначе
     */
    bool addGraph(Graph* graph, QString source);

    /*! Возвращает узел по названию
      \name nodename название
      \return Node* если все ок, 0 в противном случае
      */
    Node* node(QString nodename);
    /*! Возвращает узел по номеру
      \name id номер
      \return Node* если все ок, 0 в противном случае
      */   
    Node* node(int id);
    /*! Возвращает номер по узлу
      */
    int nodeID(Node* node);
    /*! Удалить узел по идентификатору
      */
    bool remove(int nodeid);
    /*! Удалить узел по указателю
      */
    bool remove(Node* node);
    /*! Получить спискок узло принадлежащие определенному типу
      */
    QList<Node*> nodes(QString filter="");

    /*! Получить список всех связей
      */
    const QList<Graph::Connection> getConnections();

    /*! Возвращает разрешины ли циклы в графе
      */
    bool allowCyclic();
    /*!
     * \brief graphTimeLag лаг на исполнение всего графа. считает сумму всех лагов по узлам.
     * \return лаг на исполнение всех узлов в графе
     */
    virtual double globalExecutionTime();
    /*!
     * \brief estimatedFps предельное значение кадров в секунды, автоматически оцененные
     * \return
     */
    double estimatedFps();
    /*!
     * \brief dependence зависимости графа от других библиотек, разделенных ;
     * \return
     */
   // QString dependence();
signals:

    /*! Отправка сигнала в узелы помечанные как in, используется для вложенных графов
      *\sa link
      */
    void sendInside(esp data);
    /*! Передача команды всем узлам через граф
       \param key название команды
       \param value значение
      */
    void userCommandSignal(QVariant key,QVariant value);
public slots:
    /*! Установить флаг разрешения циклических циклов
      */
    void setAllowCyclic(bool value);
    /*! Передача команды всем узлам через граф
       \param key название команды
       \param value значение
      */
    virtual void userCommand(QVariant key, QVariant value);
    /*! Версия link с использованием порядковых номеров
      *\param in номер входного узла
      *\param out номер выходного узла
      */
    bool link(int in,int out);
    /*!
     * \brief link соединение узлов по их названиям
     * \param in номер входного узла
     * \param out номер выходного узла
     * \return возывращает true в случае успеха
     */
    bool link(QString in,QString out);
    /*! Отключть все связи для графа с номером id
      */
    bool unlink(int id);
    bool unlink(QString name);
    bool unlink(Node* node);

    /*! Обнуляет буфферы всех узлов
      */
    virtual void reset();
    /*!
     *  Возвращает название схемы хозяина узла
     * \param pNode указатель на узел
     * \return возвращает название хозяина узла, если он есть в списке внешних узлов, пустое значение - иначе
     */
    QString nodeOwner(Node* pNode);
protected:
    /*! Реализация метода из Node
       если в граф приходит данные то он пытаеся их обработать через
       \sa link
      */
    virtual void receiveProcess(esp data);

private:
    /*! Функция проверяет на цикличность графа. Если граф цикличен возвращает true, иначе
      *false
      * \param in входное узел
      * \param out выходной узел
      *\return true если циклический, false если не циклический
      */
    bool checkCyclic(Node* in,Node* out);

    QList<Graph::Connection> _connections;//!< Соединение узлов

    int _lastid;//!< Счетчик уникальных идентификаторов
    bool _allowCyclic;//!< Разрешать циклические графы

protected:
    QMap<int,Node*> _nodes; //!< Ассоциация между графом и
    QMap<Node*,QString> _externalNodes;//!< Ассоцияция, если узел является частью другого графа
};

template <class T> QList<T*> getNodes(base::Graph* graph,QString filter="")
{
    QList<T*> output;
    if(0==graph)
    {
        return output;
    }
    foreach(base::Node* node,graph->nodes(filter))
    {
        T* res = qobject_cast<T*>(node);
        if(0!=res)
        {
            output << res;
        }
    }
    T* res = qobject_cast<T*>(graph);
    if(0!=res&&filter.isEmpty())
    {
         output << res;
    }
    return output;
}

}

#endif // GRAPH_H
