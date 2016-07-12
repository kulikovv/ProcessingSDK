#ifndef POINTGRAYSYNCHCAMERAS_H
#define POINTGRAYSYNCHCAMERAS_H
#include "pointgraycamera.h"
#include <QFuture>

class PointGraySynchCameras : public io::capture::CameraNode
{
    Q_OBJECT
public:
    explicit PointGraySynchCameras(QObject *parent = 0);
    ~PointGraySynchCameras();
    /*! Аналог preloadframe, при вызове метода загружается изображение
      * и отправляется сигналом send
      * \param frame номер кадра который хотим загрузить
      * \return умный указатель на отправленные данные
      */
    virtual esp get(const base::timestamp frame);
signals:
    
public slots:
private:
    void run();
    void tigger(int id);
    unsigned int numCameras;
    Camera** ppCameras;
    QFuture<void> future;
};

#endif // POINTGRAYSYNCHCAMERAS_H
