// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.
#ifndef PROCESSINGFRAMEWORK_OUTPUTNODE_H
#define PROCESSINGFRAMEWORK_OUTPUTNODE_H

#include "baseionode.h"
/*! \brief Пространство имен встроенных узлов ввода вывода
    Пространство имен встроенных узлов ввода вывода. В самом пространстве имен два базовых класса
    для ввода и вывод OutputNode и SourceNode.
    \sa io::text io::capture io::media io::kinect
  */
namespace io{
/*! Узел для записи данных
  * аналог AbstractDataOutput
  * \sa AbstractDataOutput
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT OutputNode:public BaseIONode
{
    Q_OBJECT
    public:
    explicit OutputNode(QObject *parent = 0);
    ~OutputNode();
 public slots:
    /*! Начать запись
    */
    virtual void start();
    /*! Окончить запись
    */
    virtual void stop();
    virtual void setEnable( bool value );
    virtual QIcon getIcon();
protected:
    /*! Переопределяемая функция обработки данных
      * т.к. скорость вызова виртуальной функции >> чем вызов виртуального СЛОТа то
      * мы ее выделили
      */
    virtual void receiveProcess(esp data)=0;

};
}
#endif // OUTPUTNODE_H
