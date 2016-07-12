// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.
#ifndef PROCESSINGFRAMEWORK_SOURCENODE_H
#define PROCESSINGFRAMEWORK_SOURCENODE_H

#include <io/baseionode.h>
namespace io
{
/*! SourceNode упрощенный вариант AbstractSourceInterface он наследует базовую
  * логику от класса Node. Представляет собой источник данных.
  * \sa AbstractSourceInterface
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT SourceNode:public BaseIONode
{
    Q_OBJECT
    Q_PROPERTY( long lenght READ lenght)
    Q_PROPERTY( double fps READ fps)
    public:
        explicit SourceNode(QObject *parent = 0);
        ~SourceNode();
        /*! Аналог preloadframe, при вызове метода загружается изображение
          * и отправляется исгналом send
          * \param frame номер кадра который хотим загрузить
          * \return умный указатель на отправленные данные
          */
        virtual esp get(const unsigned long frame)=0;
        /*! Длинна записи, если это запись
          * \return динну записи, если 0 значит живое видео
          */
        long lenght();
        /*! Возвращает Кол-во кадров в секунду
          * \return кол-во кадров в секунду
          */
        double fps();
     signals:
        /*! Сигнал срабатывает по вызове функции setFps либо по вызову пользователя
          */
        void fpsChanged(double newfps);
        /*! Сигнал срабатывает по вызове функции setLength либо по вызову пользователя
          */
        void lengthChanged(long newlenght);
public slots:
        virtual QIcon getIcon();
protected:
    /*! Переопределяемая функция обработки данных
      * т.к. скорость вызова виртуальной функции >> чем вызов виртуального СЛОТа то
      * мы ее выделили
      */
    virtual void receiveProcess(esp data);
    /*! Меняет fps источника
    */
    void setFps(double fps);
    /*! Меняет к-во кадров источника
    */
    void setLength(unsigned long length);
    double _fps;//!< Хранит количество кадров в секунду
    long _length;//!< Хранить кол-во кадров в последовательности
};
}

#endif // SOURCENODE_H
