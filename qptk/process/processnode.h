// This file is part of EthoStudioSDK, a C++ library for image processing.
//
// Copyright (C) 2011 Victor Kulikov <kulikov.victor@gmail.com> All rights reserved.
#ifndef PROCESSINGFRAMEWORK_PROCESSNODE_H
#define PROCESSINGFRAMEWORK_PROCESSNODE_H

#include <base/node.h>
/*! \brief Базовые классы для обработки.
    Пространство имен обработчиков данных. Здесь базовые классы для обработки.
  */
namespace process
{
/*! Узел обработки данных
  * аналог AbstractProcessor
  * \sa AbstractProcessor
  */
class PROCESSINGFRAMEWORK_SHARED_EXPORT ProcessNode:public base::Node
{
    Q_OBJECT
public:
    explicit ProcessNode(QObject *parent = 0);
    ~ProcessNode();
signals:
public slots:
protected:
    /*! Переопределяемая функция обработки данных
      * т.к. скорость вызова виртуальной функции >> чем вызов виртуального СЛОТа то
      * мы ее выделили
      */
    virtual void receiveProcess(esp data)=0;
};
}
#endif // PROCESSNODE_H
