#ifndef RESIZENODE_H
#define RESIZENODE_H

#include <process/processnode.h>

namespace process{
class ResizeNode : public ProcessNode {
    Q_OBJECT
    Q_PROPERTY(int width READ width WRITE setWidth)
    Q_PROPERTY(int height READ height WRITE setHeight)
public:
    explicit ResizeNode(QObject *parent);
    ~ResizeNode();
    int width();
    int height();
public slots:
    virtual QIcon getIcon();
    void setWidth(int value);
    void setHeight(int value);
protected:
    /*! Procesing in this fuction
      */
    virtual void receiveProcess(esp data);
    int _width,_height;
};
}
#endif // RESIZENODE_H


