#ifndef PARALLEL_H
#define PARALLEL_H
#include <workflow/workflownode.h>
#include <QMap>
/*! \brief Пространство имен для распараллеливания и замены узлов
  Узлы позволяющие распараллелить
  */
namespace workflow
{
class PROCESSINGFRAMEWORK_SHARED_EXPORT Parallel : public WorkflowNode
{
    Q_OBJECT
public:
    explicit Parallel(QObject *parent = 0);
    ~Parallel();
    Q_PROPERTY(int threadCount READ threadCount WRITE setThreadCount)
    int threadCount()
    {
        return _threadcount;
    }
signals:

public slots:
    void setThreadCount(int value);
    virtual void setNodesNames(QString value);
private slots:
    void innerReceive(esp data);
protected:
    void initNodes(int value);
    virtual void receiveProcess(esp data);
    void doParallel();
    QMap<QString,base::timestamp> _innerdata;
    QMap<QString,int> _counter;
    base::Node **_parnode;
    esp *_datacont;
    int _threadcount;
    int _courent;
};
}
#endif // PARALLEL_H
