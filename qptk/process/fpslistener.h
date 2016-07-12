#ifndef FPSLISTENER_H
#define FPSLISTENER_H

#include <QObject>
#include <QElapsedTimer>
#include <process/processnode.h>
namespace process{
    class FpsListener : public ProcessNode
    {
        Q_OBJECT
        Q_PROPERTY(double fps READ fps)
    public:
        explicit FpsListener(QObject *parent = 0);
        double fps();
    signals:
        /*! Отправляет значение в
         */
        void sendFps(double fps);
    public slots:

    protected:
        /*!
         * \brief receiveProcess
         * \param data
         */
        virtual void receiveProcess(esp data);
        double _lasttime;//!< Время последнего вызова
        double _fps;
    };
}

#endif // FPSLISTENER_H
