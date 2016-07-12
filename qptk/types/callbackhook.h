#ifndef CALLBACKHOOK_H
#define CALLBACKHOOK_H

#include <QObject>
#include "rgb.h"
#include "rgbd.h"
#include "general_tracking.h"
#include <sdk_global.h>
#include <QSharedPointer>

namespace types{

class PROCESSINGFRAMEWORK_SHARED_EXPORT CallbackHook : public QObject
{
    Q_OBJECT
public:
    explicit CallbackHook(QObject *parent = 0);
    ~CallbackHook();
signals:
    void imageCallback(types::Rgb img);
    void depthCallback(types::Rgbd depth);
    void trackingCallback(types::TrackedObjects objects);
    void trackingCallback(types::TrackedObjects3d objects);
    void trackingCallback(types::Objects objects);
};

}

#endif // CALLBACKHOOK_H
