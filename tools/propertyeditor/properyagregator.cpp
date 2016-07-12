#include "properyagregator.h"
#include <QMetaObject>
#include <QMetaProperty>
#include <QDebug>
void ProperyAgregator::accept()
{
    foreach(QString str,_ppry.keys()){
        _ppry.value(str)->setProperty(str.toUtf8(),property(str.toUtf8()));
    }
}

ProperyAgregator::ProperyAgregator(QList<QObject *> nodes, QObject *parent) : QObject(parent)
{
    foreach(QObject* node,nodes){
        const QMetaObject *meta = node->metaObject();
        qDebug() << node->objectName();
        for (int i = 0; i < meta->propertyCount(); i++)
        {


            QMetaProperty property = meta->property(i);
            qDebug() << "\t" << QString(property.name()) << property.isUser();
            if(property.isUser()){
                const char* name = property.name();
                 this->setProperty(name,node->property(name));
                _ppry.insert(QString(name),node);
               // this->setProperty(QString(node->objectName()+"/"+QString(property.name())).toUtf8(),node->property(name));
            }
        }
    }
}
