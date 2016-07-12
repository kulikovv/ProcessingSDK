#ifndef PROCESSINGFRAMEWORK_LEADERSYNCH_H
#define PROCESSINGFRAMEWORK_LEADERSYNCH_H

#include <synch/synch.h>
#include <QTimer>
namespace io
{
    class SourceNode;
}
namespace synch
{
class PROCESSINGFRAMEWORK_SHARED_EXPORT LeaderSynch : public Synch
{
    Q_OBJECT
    Q_CLASSINFO("current",tr("The current frame of the sequence"))
    Q_PROPERTY(unsigned long current READ current)
    Q_CLASSINFO("loop",tr("The loop is used when you need to create a cycle"))
    Q_PROPERTY(bool loop READ loop WRITE setLoop)
public:
    explicit LeaderSynch(QObject* obj=0);
    virtual bool isRunning();
    unsigned long current();
    bool loop() const;
private:

public slots:
    void setLoop(bool value);
    virtual void start();
    virtual void pause();
    virtual void step();
    virtual void stop();
    virtual void seek(unsigned long pos);
private slots:
    /*!
     * \brief run loop functon
     * on every tic of timer the function push all source node to execute
     */
    void run();
    void onFpsChange(double fps);//<! Recive signal when fps is changed
private:
    unsigned long _current;//!< frame counter
    QList<io::SourceNode*>  _sources;//!< the list of sources
    bool _loop; //<! use loop frames from zero if length !=0
};
}
#endif // LEADERSYNCH_H
