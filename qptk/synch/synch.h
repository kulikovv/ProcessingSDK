#ifndef PROCESSINGFRAMEWORK_SYNCH_H
#define PROCESSINGFRAMEWORK_SYNCH_H
#define SAVED_TIMES_BUFFER

#include <base/graph.h>
#include <QTimer>
namespace io
{
class SourceNode;
}
using namespace base;
/*! \brief Механизмы вызова в динамики графов qptk
    Простанство имен содержит различные механизмы для организации потока данных.
  */
namespace synch
{
/*! Абстарктный класс синхронизатора определяет интерфейс
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT Synch : public base::Graph
{
    Q_OBJECT
    Q_CLASSINFO("fps",tr("Number of desired fps used by the node"))
    Q_PROPERTY(double fps READ fps WRITE setFps USER true)
    Q_CLASSINFO("length",tr("Number of frames in the source of 0 if it is infinte"))
    Q_PROPERTY(long length READ length WRITE setLength)

public:

    /** Политики синхронизации данных.
         *  Для синхронизации данных с различных источников необходимо
         * имееть несколько алгоритмов привязки данных друг к другу.
         */
    enum SyncPolicy{
        None,       /**< Не синхронизируем вообще прогоняется все время всех файлов*/
        Longest,    /**< Все данные привязываются к самому длинному источнику данных */
        Medium,     /**< Создается средний источник данных к которому все остальные привязвыются */
        Smallest,   /**< Синхронизация с самым коротким */
        LeaderSource /**< Пользователь задает источник Лидер и Последователей (еще не реализованно) */
    };

    Synch(QObject* obj=0);
    double fps() const;
    long length() const;
    /*! Испольняется ли граф на данный момент
    */
    virtual bool isRunning()=0;

    static base::timestamp getPulseStartTime();
    /*! Добавляет узел в граф
      */
    virtual int add(Node* node,QString source="");
public slots:
   virtual QIcon getIcon();
   /*! Запускает синхронизацию
   */
   virtual void start()=0;
    /*! Преостанавлевает исполнение
    */
   virtual void pause()=0;
    /*! Остановить синхронизацию
      */
   virtual void stop()=0;
    /*!
      */
   virtual void seek(unsigned long pos)=0;

   virtual void step()=0;

signals:
    /*! Вызывается когда бенук достиг конца
      */
    void finished();
    /*! Вызывается когда синхронизатор остановлен
      */
    void stopped();
    /*! Текущий кадр в общей длине
      *\param x номер кадра
      *\sa lengthChanged
      */
    void currentFrame(unsigned long x);
    /*!
      \param l - новая длинна общего файла
      */
    void lengthChanged(unsigned long l);
    /*!
      \param вызывается при изменения fps
      */
    void fpsChanged(double newfps);
public slots:
    void setFps(double fps);
    void setLength(long length);
    void onStatus(QString status,unsigned long value,Node::Status stats);
protected:
    QList<io::SourceNode*> getSources();
    /*! Вычисляет интервалы времени с которым должны работать таймеры чтобы
      * синхронно паказывать источники
      */
    QList<unsigned long> calcIntervals(unsigned long lenght);
    /*! Один таймер пропускаются лишние кадры
      *\param fps кол-во кадров в секунду
      *\return шаги с которо\ыми надо брать следующий кадр
      */
    QList<unsigned long> calcSteps(int fps);

    io::SourceNode* findLongest();
    io::SourceNode* findSmallest();
    int averageFps();
    unsigned long averageLength();


    QTimer timer;
    double _fps;//!< Скорость кадров в синхронном режиме
    unsigned long _length;//!< Длинна в синхрнном режиме
    SyncPolicy _policy;
};
}
#endif // SYNCH_H
