#ifndef HUMANTRACKERIMPL_H
#define HUMANTRACKERIMPL_H
#include "humantracker.h"
#include <synch/synch.h>

class MessageInterceptor: public QObject{
    Q_OBJECT
    public:
        MessageInterceptor(TrackerCallback callback);
    public slots:
        void onGraphData(esp data);
    private:
        TrackerCallback _callback;
};

class HumanTrackerImpl : public HumanTracker
{

public:
    HumanTrackerImpl(TrackerCallback callback,double thresh);
    ~HumanTrackerImpl();
    virtual void setThreshold(double thresh);
    virtual void start();
    virtual void stop();
    virtual void reset();

private:
    synch::Synch* _sync;
    MessageInterceptor _int;
};

#endif // HUMANTRACKERIMPL_H
